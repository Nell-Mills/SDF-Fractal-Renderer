/********************
 * Fractal Renderer *
 ********************/

// Library includes:
#include <stdio.h>

// Local includes:
#include "Vulkan/00-Vulkan-API.h"
#include "SDF/SDF-3D.h"
#include "Utility/Input.h"
#include "Utility/Mandelbrot-Iterations.h"
#include "Utility/Utility.h"
#include "Utility/Vectors.h"

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

	// -1 = 2D Mandelbrot, 0 = Mandelbulb, 1 = Hall of Pillars, 2 = Multiple Mandelbulb:
	int fractal_type = -1;
	int sdf_type = -1;	// -1 = none, 0 = 3D, 1 = 2D.
	int animation = 0;	// 0 = No animation, -1 = Backwards, 1 = Forwards;
	if (argc > 1)
	{
		if (argv[1][0] == '0') { fractal_type = 0; }
		else if (argv[1][0] == '1') { fractal_type = 1; }
		else if (argv[1][0] == '2') { fractal_type = 2; }
	}
	if ((argc > 2) && (fractal_type != -1))
	{
		if (argv[2][0] == '0') { sdf_type = 0; }
		else if (argv[2][0] == '1') { sdf_type = 1; }
	}
	if (argc > 3)
	{
		if (argv[3][0] == '0') { animation = 0; }
		else if (argv[3][0] == '1') { animation = 1; }
		else { animation = -1; }
	}

	// Print iterations of the Mandelbrot set:
	//print_mandelbrot_2d_iterations(0.3f, 0.05f, -1);

	// Initialize the program state:
	FracRenderProgramState program_state;
	if (fractal_type == 0)
	{
		// Mandelbulb:
		program_state.position	= initialize_vector_3(0.f, -2.f, -4.f);
		program_state.front	= normalize(initialize_vector_3(0.f, 0.45f, 1.f));
	}
	else if (fractal_type == 1)
	{
		// Hall of pillars:
		program_state.position	= initialize_vector_3(25.f, 20.f, 9.f);
		program_state.front	= normalize(initialize_vector_3(0.f, 0.45f, 1.f));
	}
	else if (fractal_type == 2)
	{
		// Multiple Mandelbulb:
		program_state.position	= initialize_vector_3(0.f, -2.f, -4.f);
		program_state.front	= normalize(initialize_vector_3(0.f, 0.65f, 1.f));
	}
	else
	{
		// 2D Mandelbrot set:
		program_state.position	= initialize_vector_3(0.25f, 0.f, 0.f);
		program_state.front	= normalize(initialize_vector_3(0.f, 0.f, 1.f));
	}
	program_state.up			= initialize_vector_3(0.f, 1.f, 0.f);
	program_state.last_update		= 0.0;
	program_state.current_update		= 0.0;
	program_state.delta_t			= 0.0;
	program_state.base_movement_speed	= 1.5f;
	program_state.mouse_sensitivity		= 7.5f;
	program_state.fractal_type		= fractal_type;
	if ((fractal_type == 0) || (fractal_type == 2))
	{
		// Mandelbulb (or multiple):
		program_state.fractal_parameter_min	= 2.f;
		program_state.fractal_parameter_max	= 16.f;
	}
	else if (fractal_type == 1)
	{
		// Hall of pillars:
		program_state.fractal_parameter_min	= 2.f;
		program_state.fractal_parameter_max	= 16.f;
	}
	else
	{
		// 2D Mandelbrot set:
		program_state.fractal_parameter_min	= 2.f;
		program_state.fractal_parameter_max	= 16.f;
	}

	// Initialize 3D SDF:
	FracRenderSDF3D sdf_3d;
	if (sdf_type == 0)
	{
		if ((fractal_type == 0) || (fractal_type == 2))
		{
			sdf_3d.levels		= 8;
			sdf_3d.num_voxels	= pow(8, sdf_3d.levels);
			sdf_3d.size		= 2.f;
		}
		else
		{
			sdf_3d.levels		= 8;
			sdf_3d.num_voxels	= pow(8, sdf_3d.levels);
			sdf_3d.size		= 2.f;
		}
		sdf_3d.centre		= program_state.position;
		sdf_3d.fractal_type	= fractal_type;
		sdf_3d.voxels		= NULL;

		// Create 3D SDF:
		if (create_sdf_3d(&sdf_3d) != 0)
		{
			destroy_sdf_3d(&sdf_3d);
			return -1;
		}
	}

	// Initialize the scene UBO:
	FracRenderVulkanSceneUniform scene_uniform;
	if (sdf_type == 0)
	{
		scene_uniform.sdf_3d_centre		= sdf_3d.centre;
		scene_uniform.sdf_3d_size		= sdf_3d.size;
		scene_uniform.sdf_3d_levels		= sdf_3d.levels;
	}
	else
	{
		scene_uniform.sdf_3d_centre		= initialize_vector_3(0.f, 0.f, 0.f);
		scene_uniform.sdf_3d_size		= 0.f;
		scene_uniform.sdf_3d_levels		= 0;
	}
	if ((fractal_type == 0) || (fractal_type == 2))
	{
		// Mandelbulb (or multiple):
		scene_uniform.fractal_parameter = 8.f;
		scene_uniform.view_distance = 256.f;
	}
	else if (fractal_type == 1)
	{
		// Hall of Pillars:
		scene_uniform.fractal_parameter = 8.f;
		scene_uniform.view_distance = 16384.f;
	}
	else
	{
		// 2D Mandelbrot set:
		scene_uniform.fractal_parameter = 8.f;
		scene_uniform.view_distance = 0.f;
	}

	// Initialize Volk:
	if (initialize_volk() != 0)
	{
		return -1;
	}

	// Initialize Vulkan structs:
	FracRenderVulkanBase base;
	FracRenderVulkanDevice device;
	FracRenderVulkanValidation validation;
	FracRenderVulkanSwapchain swapchain;
	FracRenderVulkanDescriptors descriptors;
	FracRenderVulkanPipeline pipeline;
	FracRenderVulkanFramebuffers framebuffers;
	FracRenderVulkanCommands commands;

	initialize_vulkan_structs(&base, &device, &validation, &swapchain, &descriptors, &pipeline,
						&framebuffers, &commands, fractal_type, sdf_type);

	#ifdef FRACRENDER_DEBUG
		// Check support for required validation layers:
		if (check_validation_support(&validation) != 0)
		{
			destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline,
									&framebuffers, &commands);
			return -1;
		}
	#endif

	// Initialize Vulkan base (instance, window, surface and debug messenger):
	if (initialize_vulkan_base(&base, &validation) != 0)
	{
		destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline,
								&framebuffers, &commands);
		return -1;
	}

	// Select physical device and create logical device:
	if (initalize_vulkan_device(&base, &device) != 0)
	{
		destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline,
								&framebuffers, &commands);
		return -1;
	}

	// Create swapchain:
	if (initialize_vulkan_swapchain(&base, &device, &swapchain) != 0)
	{
		destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline,
								&framebuffers, &commands);
		return -1;
	}

	// Initialize descriptor layouts and sampler:
	if (initialize_vulkan_descriptor_layouts(&device, &descriptors, &sdf_3d, sdf_type) != 0)
	{
		destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline,
								&framebuffers, &commands);
		return -1;
	}

	// Create pipelines and render passes:
	if (initialize_vulkan_pipeline(&device, &swapchain, &descriptors, &framebuffers,
								&pipeline, sdf_type) != 0)
	{
		destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline,
								&framebuffers, &commands);
		return -1;
	}

	// Create framebuffers and G-buffer:
	if (initialize_vulkan_framebuffers(&device, &swapchain, &pipeline,
					&framebuffers, sdf_type) != 0)
	{
		destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline,
								&framebuffers, &commands);
		return -1;
	}

	// Create descriptors:
	if (initialize_vulkan_descriptors(&device, &framebuffers, &descriptors, sdf_type) != 0)
	{
		destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline,
								&framebuffers, &commands);
		return -1;
	}

	// Create command pool, fences and semaphores:
	if (initialize_vulkan_commands(&device, &swapchain, &commands) != 0)
	{
		destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline,
								&framebuffers, &commands);
		return -1;
	}

	if (sdf_type == 0)
	{
		// Copy SDF data into GPU buffer:
		if (copy_sdf_3d_data(&device, &descriptors, &commands, &sdf_3d) != 0)
		{
			destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline,
									&framebuffers, &commands);
			destroy_sdf_3d(&sdf_3d);
			return -1;
		}

		#ifdef FRACRENDER_DEBUG
			// Print some voxels for debugging:
			print_sdf_3d_voxels(&sdf_3d);
		#endif

		// Buffer has been copied to GPU memory so destroy CPU structure:
		destroy_sdf_3d(&sdf_3d);
	}
	else if (sdf_type == 1)
	{
		// Initialize 2D SDF image to zero values:
		if (initialize_sdf_2d_image(&device, &swapchain, &framebuffers, &commands) != 0)
		{
			return -1;
		}
	}

	#ifdef FRACRENDER_DEBUG
		// Print all Vulkan struct values for debugging:
		print_vulkan_handles(&base, &device, &validation, &swapchain, &descriptors,
							&pipeline, &framebuffers, &commands);
	#endif

	// Set GLFW callback functions (no mouse movement for 2D Mandelbrot):
	glfwSetKeyCallback(base.window, &glfw_callback_key_press);
	if (sdf_type != 2) { glfwSetCursorPosCallback(base.window, &glfw_callback_mouse_position); }

	// Set GLFW user pointer to point to the program state:
	glfwSetWindowUserPointer(base.window, &program_state);

	// Print title:
	print_title();

	// Print which type of fractal and SDF are being used:
	print_fractal_and_sdf_type(fractal_type, sdf_type);

	// Print keyboard controls:
	print_controls();

	// Tracking program state:
	int recreate_swapchain = -1;	// 0 = Yes, -1 = No.

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

			if (changes == -1)
			{
				// Error code:
				break;
			}
			else if (changes == 1)
			{
				changed_extent = 0;
			}
			else if (changes == 2)
			{
				changed_format = 0;
			}
			else if (changes == 3)
			{
				changed_extent = 0;
				changed_format = 0;
			}

			// If format changed, recreate render passes:
			if (changed_format == 0)
			{
				if (recreate_vulkan_render_passes(&device, &swapchain,
						&framebuffers, &pipeline, sdf_type) != 0)
				{
					break;
				}
			}

			// If extent changed, recreate G-buffer and 2D SDF images and views:
			if (changed_extent == 0)
			{
				if (recreate_vulkan_g_buffer_images(&device,
					&swapchain, &framebuffers) != 0)
				{
					break;
				}

				// Update G-buffer descriptors:
				update_vulkan_g_buffer_descriptors(&device,
						&framebuffers, &descriptors);

				if (sdf_type == 1)
				{
					// Recreate 2D SDF image:
					if (recreate_sdf_2d_image(&device, &swapchain,
								&framebuffers) != 0)
					{
						break;
					}

					// Re-initialize image to 0:
					if (initialize_sdf_2d_image(&device, &swapchain,
							&framebuffers, &commands) != 0)
					{
						break;
					}

					// Update 2D SDF descriptor:
					if (update_sdf_2d_descriptor(&device, &descriptors,
									&framebuffers) != 0)
					{
						break;
					}
				}
			}

			// Recreate swapchain framebuffers:
			if (recreate_vulkan_swapchain_framebuffers(&device, &swapchain,
					&pipeline, &framebuffers, num_images) != 0)
			{
				break;
			}

			// Recreate G-buffer:
			if (recreate_vulkan_g_buffer(&device, &swapchain,
				&pipeline, &framebuffers, sdf_type) != 0)
			{
				break;
			}

			// If extent changed, recreate pipelines:
			if (changed_extent == 0)
			{
				if (recreate_vulkan_pipelines(&device, &swapchain,
							&pipeline, sdf_type) != 0)
				{
					break;
				}
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
		update_scene_uniform(&base, &device, &swapchain, &scene_uniform,
							&animation);

		// Wait for a command buffer to be available:
		if (vkWaitForFences(device.logical_device, 1, &commands.fences[image_index],
							VK_TRUE, UINT64_MAX) != VK_SUCCESS)
		{
			fprintf(stderr, "Error: Unable to get free command buffer %d!\n",
									image_index);
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
		if (record_commands(&swapchain, &descriptors, &pipeline, &framebuffers,
				&commands, &scene_uniform, sdf_type, image_index) != 0)
		{
			break;
		}

		// Submit commands:
		if (submit_commands(&device, &commands, image_index) != 0)
		{
			break;
		}

		// Present results:
		int present_result = present_results(&device, &swapchain, &commands, image_index);
		if (present_result == -1)
		{
			break;
		}
		else if (present_result == 1)
		{
			// Swapchain needs recreating:
			recreate_swapchain = 0;
		}

		// Update the time:
		program_state.last_update = program_state.current_update;
	}

	// Wait for Vulkan commands to finish:
	vkDeviceWaitIdle(device.logical_device);

	// Destroy Vulkan structs:
	destroy_vulkan_structs(&base, &device, &swapchain, &descriptors, &pipeline, &framebuffers,
											&commands);

	return 0;
}
