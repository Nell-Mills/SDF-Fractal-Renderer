#include "Utility.h"

// Print title:
void print_title()
{
	printf("****************************************");
	printf("****************************************\n");

	printf("*******************************");
	printf(" Fractal Renderer ");
	printf("*******************************\n");

	printf("****************************************");
	printf("****************************************\n\n");
}

// Print fractal and optimization type:
void print_fractal_and_optimization_type(FracRenderProgramState *program_state)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	if (program_state->fractal_type == 0)
	{
		printf("Displaying Mandelbulb fractal with");
		if (program_state->optimize == 0) { printf(" 3D Signed Distance Field.\n"); }
		else if (program_state->optimize == 1) { printf(" Temporal Caching.\n"); }
		else { printf(" no optimizations.\n"); }
	}
	else if (program_state->fractal_type == 1)
	{
		printf("Displaying Hall of Pillars fractal with");
		if (program_state->optimize == 0) { printf(" 3D Signed Distance Field.\n"); }
		else if (program_state->optimize == 1) { printf(" Temporal Caching.\n"); }
		else { printf(" no optimizations.\n"); }
	}
	else
	{
		printf("Displaying 2D Mandelbrot set with no optimizations.\n");
	}
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
}

// Print keyboard controls:
void print_controls()
{
	// Title:
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Controls:\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n");

	// Keyboard controls:
	printf("W:\t\tForward\n");
	printf("A:\t\tLeft\n");
	printf("S:\t\tBackward\n");
	printf("D:\t\tRight\n");
	printf("E:\t\tUp\n");
	printf("Q:\t\tDown\n");
	printf("P:\t\tPrint current position and camera front\n");
	printf("Numpad +:\tSpeed Up\n");
	printf("Numpad -:\tSlow Down\n");
	printf("Numpad *:\tReset Speed\n");

	// End:
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
}

// Set up program state according to program inputs:
void set_up_program_state(int argc, char **argv, FracRenderProgramState *program_state)
{
	program_state->fractal_type = -1;
	program_state->optimize = -1;
	program_state->animation = -1;
	program_state->performance = -1;
	if (argc > 1)
	{
		// Fractal type. -1 = 2D Mandelbrot, 0 = Mandelbulb, 1 = Hall of Pillars.
		if (argv[1][0] == '0') { program_state->fractal_type = 0; }
		else if (argv[1][0] == '1') { program_state->fractal_type = 1; }
	}
	if ((argc > 2) && (program_state->fractal_type != -1))
	{
		// Optimization type. -1 = none, 0 = 3D SDF, 1 = Temporal Cache.
		if (argv[2][0] == '0') { program_state->optimize = 0; }
		else if (argv[2][0] == '1') { program_state->optimize = 1; }
	}
	if (argc > 3)
	{
		// Animation. -1 = No animation, 0 = Parameter, 1 = Flythrough, 2 = Artefacts.
		if (argv[3][0] == '0') { program_state->animation = 0; }
		else if (argv[3][0] == '1') { program_state->animation = 1; }
		else if (argv[3][0] == '2') { program_state->animation = 2; }
	}
	if (argc > 4)
	{
		// Performance. -1 = No measurements, 0 = One-Shot, 1 = Multiple.
		if (argv[4][0] == '0') { program_state->performance = 0; }
		else if (argv[4][0] == '1') { program_state->performance = 1; }
	}

	// Check fractal type and animation (no special animations for Mandelbrot and Mandelbulb):
	if ((program_state->fractal_type != 1) && (program_state->animation > 0))
	{
		// Revert to parameter animation:
		printf("Warning: No special animation exists for selected fractal"
				" type. Falling back to parameter animation.\n");
		program_state->animation = 0;
	}

	// Get performance file name:
	char *default_name = "./Performance-Measurements/00-Default-Name.txt";
	if (argc > 5) { strcpy(program_state->performance_file_name, argv[5]); }
	else { strcpy(program_state->performance_file_name, default_name); }

	// Set up initial position, camera front and fractal parameter range:
	if (program_state->fractal_type == 0)
	{
		// Mandelbulb:
		program_state->position = initialize_vector_3(-1.3f, -1.2f, 2.f);
		program_state->front = normalize(initialize_vector_3(0.5f, 0.45f, -0.75f));

		program_state->fractal_parameter = 8.f;
		program_state->fractal_parameter_start = 10.f;
		program_state->fractal_parameter_min = 4.f;
		program_state->fractal_parameter_max = 16.f;
	}
	else if (program_state->fractal_type == 1)
	{
		// Hall of pillars:
		if (program_state->animation == 0)
		{
			// Position allows camera to be enclosed in shrinking room:
			program_state->position = initialize_vector_3(-1250.f, -1700.f, -1400.f);
			program_state->front = normalize(initialize_vector_3(0.9f, 0.f, 0.5f));
		}
		else
		{
			// Normal:
			program_state->position = initialize_vector_3(25.f, 20.f, 9.f);
			program_state->front = normalize(initialize_vector_3(0.f, 0.45f, 1.f));
		}

		program_state->fractal_parameter = 2.f;
		program_state->fractal_parameter_start = 2.f;
		program_state->fractal_parameter_min = 1.6f;
		program_state->fractal_parameter_max = 2.4f;
	}
	else
	{
		// 2D Mandelbrot set:
		program_state->position = initialize_vector_3(0.25f, 0.f, 0.f);
		program_state->front = normalize(initialize_vector_3(0.f, 0.f, 1.f));

		program_state->fractal_parameter = 0.f;
		program_state->fractal_parameter_start = 0.f;
		program_state->fractal_parameter_min = -0.75f;
		program_state->fractal_parameter_max = 0.75f;
	}

	// Set up other initial values for program state:
	program_state->up			= initialize_vector_3(0.f, 1.f, 0.f);
	program_state->last_update		= 0.0;
	program_state->current_update		= 0.0;
	program_state->delta_t			= 0.0;
	program_state->frames			= 0;
	program_state->frame_time		= 0.0;
	program_state->animation_frames		= 0;
	program_state->max_animation_frames	= 0;
	program_state->base_movement_speed	= 1.5f;
	program_state->mouse_sensitivity	= 7.5f;
}

// Set up scene uniform object:
void set_up_scene_uniform(FracRenderProgramState *program_state, FracRenderSDF3D *sdf_3d,
					FracRenderVulkanSceneUniform *scene_uniform)
{
	// Set up 3D SDF information:
	if (program_state->optimize == 0)
	{
		// 3D SDF:
		scene_uniform->sdf_3d_centre	= sdf_3d->centre;
		scene_uniform->sdf_3d_size	= sdf_3d->size;
		scene_uniform->sdf_3d_levels	= sdf_3d->levels;
	}
	else
	{
		// No 3D SDF:
		scene_uniform->sdf_3d_centre	= initialize_vector_3(0.f, 0.f, 0.f);
		scene_uniform->sdf_3d_size	= 0.f;
		scene_uniform->sdf_3d_levels	= 0;
	}

	// Set up fractal information:
	if (program_state->fractal_type == 0)
	{
		// Mandelbulb:
		scene_uniform->view_distance = 256.f;
	}
	else if (program_state->fractal_type == 1)
	{
		// Hall of Pillars:
		scene_uniform->view_distance = 16384.f;
	}
	else
	{
		// 2D Mandelbrot set:
		scene_uniform->view_distance = 0.f;
	}
}

// Initialize Vulkan structs to default values:
void initialize_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanValidation *validation, FracRenderVulkanSwapchain *swapchain,
	FracRenderVulkanDescriptors *descriptors, FracRenderVulkanPipeline *pipeline,
	FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanCommands *commands,
	FracRenderVulkanPerformance *performance, FracRenderProgramState *program_state)
{
	// Vulkan base:
	base->instance		= VK_NULL_HANDLE;
	base->window		= NULL;
	base->surface		= VK_NULL_HANDLE;

	base->debug_messenger	= VK_NULL_HANDLE;

	// Device:
	device->physical_device		= VK_NULL_HANDLE,
	device->logical_device		= VK_NULL_HANDLE,

	device->num_device_extensions	= 1;
	device->device_extensions[0]	= VK_KHR_SWAPCHAIN_EXTENSION_NAME;

	device->graphics_family_index	= 100;
	device->graphics_queue		= VK_NULL_HANDLE;
	device->present_family_index	= 100;
	device->present_queue		= VK_NULL_HANDLE;

	// Validation:
#ifdef FRACRENDER_DEBUG
	validation->num_validation_layers	= 1;
	validation->num_validation_extensions	= 1;
	validation->validation_layers[0]	= "VK_LAYER_KHRONOS_validation";
	validation->validation_extensions[0]	= "VK_EXT_debug_utils";
#else
	validation->num_validation_layers	= 0;
	validation->num_validation_extensions	= 0;
#endif

	// Swapchain:
	swapchain->swapchain			= VK_NULL_HANDLE;
	swapchain->num_swapchain_images		= 0;
	swapchain->swapchain_images		= NULL;
	swapchain->swapchain_image_views	= NULL;
	swapchain->swapchain_format		= VK_FORMAT_UNDEFINED;
	swapchain->swapchain_extent.width	= 0;
	swapchain->swapchain_extent.height	= 0;

	// Descriptors:
	descriptors->descriptor_pool			= VK_NULL_HANDLE;

	descriptors->sampler				= VK_NULL_HANDLE;

	descriptors->scene_descriptor_layout		= VK_NULL_HANDLE;
	descriptors->scene_descriptor			= VK_NULL_HANDLE;
	descriptors->scene_buffer			= VK_NULL_HANDLE;
	descriptors->scene_memory			= VK_NULL_HANDLE;

	descriptors->num_g_buffer_descriptors		= 1;
	descriptors->g_buffer_descriptor_layout		= VK_NULL_HANDLE;
	descriptors->g_buffer_descriptors		= NULL;

	descriptors->sdf_3d_descriptor_layout		= VK_NULL_HANDLE;
	descriptors->sdf_3d_descriptor			= VK_NULL_HANDLE;
	descriptors->sdf_3d_buffer			= VK_NULL_HANDLE;
	descriptors->sdf_3d_memory			= VK_NULL_HANDLE;

	descriptors->temporal_cache_descriptor_layout	= VK_NULL_HANDLE;
	descriptors->temporal_cache_descriptor		= VK_NULL_HANDLE;

	// Pipeline:
	pipeline->geometry_pipeline_layout	= VK_NULL_HANDLE;
	pipeline->colour_pipeline_layout	= VK_NULL_HANDLE;

	pipeline->geometry_pipeline		= VK_NULL_HANDLE;
	pipeline->colour_pipeline		= VK_NULL_HANDLE;

	pipeline->geometry_render_pass		= VK_NULL_HANDLE;
	pipeline->colour_render_pass		= VK_NULL_HANDLE;

	pipeline->geometry_vertex_shader	= VK_NULL_HANDLE;
	pipeline->geometry_fragment_shader	= VK_NULL_HANDLE;
	pipeline->colour_vertex_shader		= VK_NULL_HANDLE;
	pipeline->colour_fragment_shader	= VK_NULL_HANDLE;

	if (program_state->fractal_type == 0)
	{
		#define SHADER_DIR_ "Assets/Shaders/Mandelbulb/"
		// Mandelbulb:
		if (program_state->optimize == 0)
		{
			// 3D SDF:
			pipeline->geometry_vertex_shader_path =
				SHADER_DIR_"Geometry-Mandelbulb-SDF-3D.vert.sprv";
			pipeline->geometry_fragment_shader_path =
				SHADER_DIR_"Geometry-Mandelbulb-SDF-3D.frag.sprv";
		}
		else if (program_state->optimize == 1)
		{
			// Temporal Cache:
			pipeline->geometry_vertex_shader_path =
				SHADER_DIR_"Geometry-Mandelbulb-Temporal-Cache.vert.sprv";
			pipeline->geometry_fragment_shader_path =
				SHADER_DIR_"Geometry-Mandelbulb-Temporal-Cache.frag.sprv";
		}
		else
		{
			// No SDF:
			pipeline->geometry_vertex_shader_path =
				SHADER_DIR_"Geometry-Mandelbulb.vert.sprv";
			pipeline->geometry_fragment_shader_path =
				SHADER_DIR_"Geometry-Mandelbulb.frag.sprv";
		}
		pipeline->colour_vertex_shader_path =
			SHADER_DIR_"Colour-Mandelbulb.vert.sprv";
		pipeline->colour_fragment_shader_path =
			SHADER_DIR_"Colour-Mandelbulb.frag.sprv";
		#undef SHADER_DIR_
	}
	else if (program_state->fractal_type == 1)
	{
		#define SHADER_DIR_ "Assets/Shaders/Hall-Of-Pillars/"
		// Hall of Pillars:
		if (program_state->optimize == 0)
		{
			// 3D SDF:
			pipeline->geometry_vertex_shader_path =
				SHADER_DIR_"Geometry-Hall-Of-Pillars-SDF-3D.vert.sprv";
			pipeline->geometry_fragment_shader_path =
				SHADER_DIR_"Geometry-Hall-Of-Pillars-SDF-3D.frag.sprv";
		}
		else if (program_state->optimize == 1)
		{
			// Temporal Cache:
			pipeline->geometry_vertex_shader_path =
				SHADER_DIR_"Geometry-Hall-Of-Pillars-Temporal-Cache.vert.sprv";
			pipeline->geometry_fragment_shader_path =
				SHADER_DIR_"Geometry-Hall-Of-Pillars-Temporal-Cache.frag.sprv";
		}
		else
		{
			// No SDF:
			pipeline->geometry_vertex_shader_path =
				SHADER_DIR_"Geometry-Hall-Of-Pillars.vert.sprv";
			pipeline->geometry_fragment_shader_path =
				SHADER_DIR_"Geometry-Hall-Of-Pillars.frag.sprv";
		}
		pipeline->colour_vertex_shader_path =
			SHADER_DIR_"Colour-Hall-Of-Pillars.vert.sprv";
		pipeline->colour_fragment_shader_path =
			SHADER_DIR_"Colour-Hall-Of-Pillars.frag.sprv";
		#undef SHADER_DIR_
	}
	else
	{
		#define SHADER_DIR_ "Assets/Shaders/Mandelbrot-2D/"
		// 2D Mandelbrot:
		pipeline->geometry_vertex_shader_path =
			SHADER_DIR_"Geometry-Mandelbrot-2D.vert.sprv";
		pipeline->geometry_fragment_shader_path =
			SHADER_DIR_"Geometry-Mandelbrot-2D.frag.sprv";
		pipeline->colour_vertex_shader_path =
			SHADER_DIR_"Colour-Mandelbrot-2D.vert.sprv";
		pipeline->colour_fragment_shader_path =
			SHADER_DIR_"Colour-Mandelbrot-2D.frag.sprv";
		#undef SHADER_DIR_
	}

	// Framebuffers:
	framebuffers->framebuffers		= NULL;

	framebuffers->g_buffer			= VK_NULL_HANDLE;

	if (program_state->optimize == 1)
	{
		framebuffers->num_g_buffer_images = 2;
	}
	else
	{
		framebuffers->num_g_buffer_images = 1;
	}
	framebuffers->g_buffer_images		= NULL;
	framebuffers->g_buffer_image_views	= NULL;

	// Allocate memory for G-buffer formats (free in destroy_vulkan_framebuffers):
	framebuffers->g_buffer_formats		= malloc(framebuffers->num_g_buffer_images *
									sizeof(VkFormat));
	if (program_state->optimize == 1)
	{
		// Positions + iterations, and distance write:
		framebuffers->g_buffer_formats[0]	= VK_FORMAT_R32G32B32A32_SFLOAT;
		framebuffers->g_buffer_formats[1]	= VK_FORMAT_R32G32B32A32_SFLOAT;
	}
	else
	{
		// Positions + iterations:
		framebuffers->g_buffer_formats[0]	= VK_FORMAT_R32G32B32A32_SFLOAT;
	}

	framebuffers->temporal_cache_image		= VK_NULL_HANDLE;
	framebuffers->temporal_cache_memory		= VK_NULL_HANDLE;
	framebuffers->temporal_cache_image_view		= VK_NULL_HANDLE;
	framebuffers->temporal_cache_format		= VK_FORMAT_R32G32B32A32_SFLOAT;

	// Commands:
	commands->command_pool		= VK_NULL_HANDLE;
	commands->command_buffers	= NULL;
	commands->fences		= NULL;
	commands->image_available	= VK_NULL_HANDLE;
	commands->render_finished	= VK_NULL_HANDLE;

	// Performance:
	performance->query_pool		= VK_NULL_HANDLE;
	performance->timestamp_period	= 0.f;
}

// Perform all setup of Vulkan environment and SDF (if any):
int set_up_vulkan(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
		FracRenderVulkanValidation *validation, FracRenderVulkanSwapchain *swapchain,
		FracRenderVulkanDescriptors *descriptors, FracRenderVulkanPipeline *pipeline,
		FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanCommands *commands,
		FracRenderVulkanPerformance *performance, FracRenderProgramState *program_state,
		FracRenderSDF3D *sdf_3d)
{
	// Initialize Volk:
	if (initialize_volk() != 0)
	{
		return -1;
	}

	// Initialize Vulkan structs to default values:
	initialize_vulkan_structs(base, device, validation, swapchain, descriptors, pipeline,
					framebuffers, commands, performance, program_state);

	#ifdef FRACRENDER_DEBUG
		// Check support for required validation layers:
		if (check_validation_support(validation) != 0)
		{
			return -1;
		}
	#endif

	// Initialize Vulkan base (instance, window, surface and debug messenger):
	if (initialize_vulkan_base(base, validation) != 0)
	{
		return -1;
	}

	// Select physical device and create logical device:
	if (initalize_vulkan_device(base, device) != 0)
	{
		return -1;
	}

	// Create swapchain:
	if (initialize_vulkan_swapchain(base, device, swapchain) != 0)
	{
		return -1;
	}

	// Initialize descriptor layouts and sampler:
	if (initialize_vulkan_descriptor_layouts(device, descriptors, sdf_3d,
					program_state->optimize) != 0)
	{
		return -1;
	}

	// Create pipelines and render passes:
	if (initialize_vulkan_pipeline(device, swapchain, descriptors, framebuffers,
					pipeline, program_state->optimize) != 0)
	{
		return -1;
	}

	// Create framebuffers and G-buffer:
	if (initialize_vulkan_framebuffers(device, swapchain, pipeline,
				framebuffers, program_state->optimize) != 0)
	{
		return -1;
	}

	// Create descriptors:
	if (initialize_vulkan_descriptors(device, framebuffers, descriptors,
					program_state->optimize) != 0)
	{
		return -1;
	}

	// Create command pool, fences and semaphores:
	if (initialize_vulkan_commands(device, swapchain, commands) != 0)
	{
		return -1;
	}

	// Set up performance measuring structures:
	if (program_state->performance > -1)
	{
		if (initialize_vulkan_performance(device, swapchain, performance) != 0)
		{
			return -1;
		}
	}

	if (program_state->optimize == 0)
	{
		// Copy SDF data into GPU buffer:
		if (copy_sdf_3d_data(device, descriptors, commands, sdf_3d) != 0)
		{
			return -1;
		}
	}
	else if (program_state->optimize == 1)
	{
		// Initialize Temporal Cache image to zero values:
		if (initialize_temporal_cache_image(device, swapchain, framebuffers, commands) != 0)
		{
			return -1;
		}
	}

	return 0;
}

// Destroy contents of Vulkan structs:
void destroy_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
		FracRenderVulkanSwapchain *swapchain, FracRenderVulkanDescriptors *descriptors,
		FracRenderVulkanPipeline *pipeline, FracRenderVulkanFramebuffers *framebuffers,
		FracRenderVulkanCommands *commands, FracRenderVulkanPerformance *performance)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Cleaning up...\n");

	// Destroy Vulkan performance structure:
	destroy_vulkan_performance(device, performance);

	// Destroy Vulkan command pool, fences and semaphores:
	destroy_vulkan_commands(device, swapchain, commands);

	// Destroy Vulkan framebuffers:
	destroy_vulkan_framebuffers(device, swapchain, framebuffers);

	// Destroy Vulkan pipeline:
	destroy_vulkan_pipeline(device, pipeline);

	// Destroy Vulkan descriptors:
	destroy_vulkan_descriptors(device, descriptors);

	// Destroy Vulkan swapchain:
	destroy_vulkan_swapchain(device, swapchain);

	// Destroy Vulkan device:
	destroy_vulkan_device(device);

	// Destroy Vulkan base:
	destroy_vulkan_base(base);

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
}
