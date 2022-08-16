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

	// Initialize program state:
	FracRenderProgramState program_state;
	set_up_program_state(argc, argv, &program_state);

	// Animation function:
	void (*animation_update_function)(FracRenderProgramState *);
	if (program_state.animation == -1) { animation_update_function = update_animation_none; }
	else if (program_state.animation == 0)
	{
		animation_update_function = update_animation_parameter;
	}
	else if (program_state.animation == 1)
	{
		// Special flythrough animation for Hall of Pillars:
		animation_update_function = update_animation_flythrough;
	}
	else
	{
		// Animation to show artefacts in Hall of Pillars:
		animation_update_function = update_animation_artefacts;
	}

	// Run animation function once to get correct number of animation frames:
	animation_update_function(&program_state);

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
	FILE *performance_file = NULL;
	if (program_state.performance > -1)
	{
		performance_file = fopen(program_state.performance_file_name, "w");
		if (!performance_file)
		{
			destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline,
							&framebuffers, &commands, &performance);
			return -1;
		}
		// Write column headers:
		if (program_state.performance == 1)
		{
			fprintf(performance_file, "Frame\tMedian(ns)\tMin(ns)\t\tMax(ns)\n\n");
		}
		else
		{
			fprintf(performance_file, "Median(ns)\tMin(ns)\t\t"
						"Max(ns)\t\tFrame Time(ns)\n\n");
		}
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
	double total_frame_time = 0.0;
	int values_captured = 0;	// Capture a certain number of values for performance.
	int max_values = 25;		// Animations take a long time to capture, only get 25.

	// Start measuring after 1000 frames to allow warm up:
	int warm_up;
	if (program_state.performance > -1) { warm_up = 0; }
	else { warm_up = 1001; }

	// Storing measurements for geometry render pass time:
	double *shader_time = NULL;
	double **multi_shader_time = NULL;
	if (program_state.performance == 0)
	{
		shader_time = malloc(1000 * sizeof(double));
		memset(shader_time, 0, 1000 * sizeof(double));
	}
	else if (program_state.performance == 1)
	{
		if (program_state.max_animation_frames == 0)
		{
			fprintf(stderr, "Error: Incompatible arguments. If performance"
				" is set to 1, animation should not be off!\n");
			fclose(performance_file);
			destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline,
							&framebuffers, &commands, &performance);
			return -1;
		}

		multi_shader_time = malloc(max_values * sizeof(double *));
		for (int i = 0; i < max_values; i++)
		{
			multi_shader_time[i] = malloc(program_state.max_animation_frames *
									sizeof(double));
			memset(multi_shader_time[i], 0, program_state.max_animation_frames *
									sizeof(double));
		}
		printf("Collecting data for %ld frames, %d times.\n\n",
			program_state.max_animation_frames, max_values);
	}

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

		// Update animation:
		if ((program_state.animation > -1) && (warm_up > 1000) &&
			(program_state.animation_frames < program_state.max_animation_frames))
		{
			animation_update_function(&program_state);
			program_state.animation_frames++;

			// Get geometry render pass execution time:
			if (program_state.performance == 1)
			{
				get_shader_time(multi_shader_time[values_captured],
					program_state.animation_frames, image_index,
					1, &device, &performance);
			}

			if (program_state.animation_frames == program_state.max_animation_frames)
			{
				// Move on to next runthrough:
				if (program_state.performance == 1)
				{
					values_captured++;
					printf("Captured: %d\n", values_captured);
				}
				if ((values_captured == max_values) &&
					(program_state.performance == 1))
				{
					printf("\nFinished performance measurements.\n\n");

					// Write out performance measurements:
					write_measurements(performance_file, multi_shader_time,
						program_state.max_animation_frames, max_values);

					break;
				}
				else
				{
					program_state.animation_frames = 0;
				}
			}
		}

		// Get geometry render pass execution time:
		if ((program_state.performance == 0) && (warm_up > 1000))
		{
			get_shader_time(shader_time, (100 * values_captured) +
				program_state.frames, image_index, 0, &device, &performance);
		}

		// Get frame rate and change window title:
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

			// Write out performance measurements:
			if ((program_state.performance == 0) && (warm_up > 1000))
			{
				total_frame_time += program_state.frame_time;

				// If enough values have been captured, turn off performance:
				values_captured++;
				printf("Progress: %d%%\n", values_captured * 10);
				if (values_captured == 10)
				{
					// Get average frame time:
					total_frame_time /= 100.0;
					total_frame_time *= 1000000000.0;

					// Take median, min, max and frame time, all in ns:
					fprintf(performance_file, "%.1lf\t", shader_time[500]);
					fprintf(performance_file, "%.1lf\t", shader_time[0]);
					fprintf(performance_file, "%.1lf\t", shader_time[999]);
					fprintf(performance_file, "%.1lf\n", total_frame_time);

					printf("\nFinished performance measurements.\n\n");
					break;
				}
			}

			program_state.frames = 0;
			program_state.frame_time = 0.0;
		}

		if (warm_up < 1000)
		{
			warm_up++;
			if (warm_up == 1000)
			{
				if (program_state.performance > -1)
				{
					printf("Starting performance measurements...\n\n");
				}
				if (program_state.animation > -1)
				{
					printf("Starting animation...\n\n");
				}
				warm_up++;
			}
		}
	}

	// Wait for Vulkan commands to finish:
	vkDeviceWaitIdle(device.logical_device);

	// Destroy Vulkan structs:
	destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline, &framebuffers,
									&commands, &performance);

	// Close performance file:
	if (program_state.performance > -1) { fclose(performance_file); }

	// Free memory:
	if (shader_time) { free(shader_time); }
	if (multi_shader_time)
	{
		for (int i = 0; i < max_values; i++)
		{
			free(multi_shader_time[i]);
		}
		free(multi_shader_time);
	}

	return 0;
}
