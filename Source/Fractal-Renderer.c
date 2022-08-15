/********************
 * Fractal Renderer *
 ********************/

// Library includes:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Local includes:
#include "Vulkan/00-Vulkan-API.h"
#include "SDF/SDF-3D.h"
#include "Utility/Utility.h"

/********
 * Main *
 ********/

int main(int argc, char **argv)
{
	// Check size of scene UBO:
	if (sizeof(FracRenderVulkanSceneUniform) > 65536)
	{
		fprintf(stderr, "Error: Size of Scene Uniform must be 65536 bytes or less!\n");
		return -1;
	}
	if (sizeof(FracRenderVulkanSceneUniform) % 4 != 0)
	{
		fprintf(stderr, "Error: Size of Scene Uniform must be a multiple of 4 bytes!\n");
		return -1;
	}

	// Animation function (choose none for no animation):
	void (*animation_update_function)(FracRenderProgramState *) = update_animation_none;
	//void (*animation_update_function)(FracRenderProgramState *) = update_animation_flythrough;

	// Initialize program state:
	FracRenderProgramState program_state;
	set_up_program_state(argc, argv, &program_state);

	// Initialize 3D SDF:
	FracRenderSDF3D sdf_3d;
	set_up_sdf_3d(&program_state, &sdf_3d);

	// Initialize scene UBO:
	FracRenderVulkanSceneUniform scene_uniform;
	set_up_scene_uniform(&program_state, &sdf_3d, &scene_uniform);

	// Declare Vulkan structs:
	FracRenderVulkanBase base;
	FracRenderVulkanDevice device;
	FracRenderVulkanValidation validation;
	FracRenderVulkanSwapchain swapchain;
	FracRenderVulkanDescriptors descriptors;
	FracRenderVulkanPipeline pipeline;
	FracRenderVulkanFramebuffers framebuffers;
	FracRenderVulkanCommands commands;
	FracRenderVulkanPerformance performance;

	// Set up Vulkan:
	if (set_up_vulkan(&base, &device, &validation, &swapchain, &descriptors, &pipeline,
		&framebuffers, &commands, &performance, &program_state, &sdf_3d) != 0)
	{
		destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline,
						&framebuffers, &commands, &performance);
		destroy_sdf_3d(&sdf_3d);
		return -1;
	}

	// Debug print functions:
	#ifdef FRACRENDER_DEBUG
		// Print iterations of the Mandelbrot set:
		//print_mandelbrot_2d_iterations(0.3f, 0.05f, -1);

		// Print some voxels for debugging:
		//print_sdf_3d_voxels(sdf_3d);

		// Print all Vulkan struct values for debugging:
		//print_vulkan_handles(base, device, validation, swapchain, descriptors,
		//			pipeline, framebuffers, commands, performance);
	#endif

	// 3D SDF buffer has been copied to GPU memory so destroy CPU structure:
	if (program_state.optimize == 0) { destroy_sdf_3d(&sdf_3d); }

	// Set up performance recording file (overwrite any old ones!):
	FILE *performance_file;
	if (program_state.performance == 0)
	{
		performance_file = fopen(program_state.performance_file_name, "w");
		if (!performance_file)
		{
			destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline,
							&framebuffers, &commands, &performance);
			return -1;
		}
		// Write column headers:
		fprintf(performance_file, "Frame\tMedian(ns)\t\tMin(ns)\t\t"
					"Max(ns)\t\tFrame Time(s)\n\n");
	}

	// Set GLFW callback functions (no mouse movement for 2D Mandelbrot):
	glfwSetKeyCallback(base.window, &glfw_callback_key_press);
	if (program_state.fractal_type != -1)
	{
		glfwSetCursorPosCallback(base.window, &glfw_callback_mouse_position);
	}

	// Set GLFW user pointer to point to the program state:
	glfwSetWindowUserPointer(base.window, &program_state);

	// Print title, optimization and fractal info, and keyboard controls:
	print_title();
	print_fractal_and_optimization_type(&program_state);
	print_controls();

	// Tracking swapchain and performance measurements:
	int recreate_swapchain = -1;	// 0 = Yes, -1 = No.

	int values_captured = 0;	// Capture a set number of values for performance.
	int max_values = 10;		// Change this value to measure more times.
	int warm_up = 0;		// Start measuring after 1000 frames to allow warm up.
	int order = 0;			// Whether to insert new measurements according to value.
	int animation_tick_max = 1;	// How often to update animation.
	int animation_tick = 0;		// Current animation update tick.

	// Storing measurements for geometry render pass time over 100 frames:
	double *shader_time = malloc(100 * sizeof(double));
	memset(shader_time, 0, 100 * sizeof(double));

	// Main loop:
	while(!glfwWindowShouldClose(base.window))
	{
		// Get current time and deltaT, and poll GLFW events and movement keys:
		program_state.current_update = glfwGetTime();
		program_state.delta_t = program_state.current_update - program_state.last_update;
		poll_movement_keys(base.window);
		glfwPollEvents();

		// Check recreate_swapchain flag:
		if (recreate_swapchain == 0)
		{
			// Save current number of swapchain images:
			uint32_t num_images = swapchain.num_swapchain_images;

			// Wait for Vulkan commands to finish:
			vkDeviceWaitIdle(device.logical_device);

			// Recreate swapchain and see what value is returned:
			int changed_extent = -1;
			int changed_format = -1;
			int changes = recreate_vulkan_swapchain(&base, &device, &swapchain);

			// -1 is error code:
			if (changes == -1) { break; }
			else if (changes == 1) { changed_extent = 0; }
			else if (changes == 2) { changed_format = 0; }
			else if (changes == 3)
			{
				changed_extent = 0;
				changed_format = 0;
			}

			// If format changed, recreate render passes:
			if (changed_format == 0)
			{
				if (recreate_vulkan_render_passes(&device, &swapchain,
					&framebuffers, &pipeline, program_state.optimize) != 0)
				{
					break;
				}
			}

			// If extent changed, recreate G-buffer and Temporal Cache images and views:
			if (changed_extent == 0)
			{
				if (recreate_vulkan_g_buffer_images(&device,
					&swapchain, &framebuffers) != 0) { break; }

				// Update G-buffer descriptors:
				update_vulkan_g_buffer_descriptors(&device,
						&framebuffers, &descriptors);

				if (program_state.optimize == 1)
				{
					// Recreate Temporal Cache image:
					if (recreate_temporal_cache_image(&device, &swapchain,
								&framebuffers) != 0) { break; }

					// Re-initialize image to 0:
					if (initialize_temporal_cache_image(&device, &swapchain,
							&framebuffers, &commands) != 0) { break; }

					// Update Temporal Cache descriptor:
					if (update_temporal_cache_descriptor(&device, &descriptors,
								&framebuffers) != 0) { break; }
				}
			}

			// Recreate swapchain framebuffers:
			if (recreate_vulkan_swapchain_framebuffers(&device, &swapchain,
				&pipeline, &framebuffers, num_images) != 0) { break; }

			// Recreate G-buffer:
			if (recreate_vulkan_g_buffer(&device, &swapchain,
				&pipeline, &framebuffers, program_state.optimize) != 0) { break; }

			// If extent changed, recreate pipelines:
			if (changed_extent == 0)
			{
				if (recreate_vulkan_pipelines(&device, &swapchain,
					&pipeline, program_state.optimize) != 0) { break; }
			}

			recreate_swapchain = -1;
		}

		// Get next swapchain image:
		uint32_t image_index = 0;
		VkResult acquisition_result = vkAcquireNextImageKHR(
			device.logical_device,
			swapchain.swapchain,
			UINT64_MAX,
			commands.image_available,
			VK_NULL_HANDLE,
			&image_index
		);

		// See if swapchain needs recreating:
		if ((acquisition_result == VK_SUBOPTIMAL_KHR) ||
			(acquisition_result == VK_ERROR_OUT_OF_DATE_KHR))
		{
			recreate_swapchain = 0;
			continue;
		}

		// See if next image was acquired:
		if (acquisition_result != VK_SUCCESS)
		{
			fprintf(stderr, "Error: Unable to get next swapchain image!\n");
			break;
		}

		// Update scene uniform:
		update_scene_uniform(&base, &device, &swapchain, &scene_uniform, &program_state);

		// Wait for a command buffer to be available:
		if (vkWaitForFences(device.logical_device, 1, &commands.fences[image_index],
							VK_TRUE, UINT64_MAX) != VK_SUCCESS)
		{
			fprintf(stderr, "Error: Unable to get command buffer %d!\n", image_index);
			break;
		}

		// Reset fence:
		if (vkResetFences(device.logical_device, 1,
			&commands.fences[image_index]) != VK_SUCCESS)
		{
			fprintf(stderr, "Error: Unable to reset fence %d!\n", image_index);
			break;
		}

		// Record commands:
		if (record_commands(&swapchain, &descriptors, &pipeline, &framebuffers, &commands,
			&performance, &scene_uniform, &program_state, image_index) != 0) { break; }

		// Submit commands:
		if (submit_commands(&device, &commands, image_index) != 0) { break; }

		// Present results. Return value of 1 means swapchain needs recreating:
		int present_result = present_results(&device, &swapchain, &commands, image_index);
		if (present_result == -1) { break; }
		else if (present_result == 1) { recreate_swapchain = 0; }

		// Update the time:
		program_state.last_update = program_state.current_update;

		/****************************
		 * PERFORMANCE MEASUREMENTS *
		 ****************************/

		// Update frame count and time:
		program_state.frames++;
		program_state.frame_time += program_state.delta_t;

		if ((program_state.performance == 0) && (warm_up >= 1000))
		{
			if (warm_up == 1000)
			{
				warm_up++;
				printf("Starting performance measurements...\n\n");
			}

			// Get geometry render pass execution time:
			get_shader_time(shader_time, program_state.frames,
				image_index, order, &device, &performance);
		}

		if (program_state.frames == 100)
		{
			// Get current frame rate (for last 100 frames):
			program_state.frame_time /= 100.0;
			double frame_rate = 1.0 / program_state.frame_time;

			// Change window title to display frame rate:
			char window_title[32];
			if (sprintf(window_title, "Fractal Renderer - %.2lf", frame_rate) < 0)
			{
				fprintf(stderr, "Error: Failed to change window title!\n");
				break;
			}
			glfwSetWindowTitle(base.window, window_title);
		}

		if (program_state.frames == 100)
		{
			// Write out performance measurements:
			if ((program_state.performance == 0) && (warm_up > 1000))
			{
				// Take median (ns), min (ns), max (ns) and frame time (s):
				fprintf(performance_file, "R %d\t", values_captured + 1);
				fprintf(performance_file, "%.1lf\t", shader_time[50]);
				fprintf(performance_file, "%.1lf\t", shader_time[0]);
				fprintf(performance_file, "%.1lf\t", shader_time[99]);
				fprintf(performance_file, "%.1lf\n\n", program_state.frame_time
										* 1000000000.0);

				// Reset times:
				memset(shader_time, 0, 100 * sizeof(double));

				// If enough values have been captured, turn off performance:
				values_captured++;
				if (values_captured == max_values)
				{
					printf("Finished performance measurements.\n\n");
					program_state.performance = -1;
				}
			}

			program_state.frames = 0;
			program_state.frame_time = 0.0;
		}

		if (warm_up < 1000) { warm_up++; }

		// Update animation:
		if (warm_up >= 1000)
		{
			animation_tick++;
			if (animation_tick == animation_tick_max)
			{
				animation_update_function(&program_state);
				program_state.animation_frames++;
				animation_tick = 0;
			}
		}
	}

	// Wait for Vulkan commands to finish:
	vkDeviceWaitIdle(device.logical_device);

	// Close performance file:
	if (program_state.performance == 0) { fclose(performance_file); }

	// Free memory:
	free(shader_time);

	// Destroy Vulkan structs:
	destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline, &framebuffers,
									&commands, &performance);

	return 0;
}
