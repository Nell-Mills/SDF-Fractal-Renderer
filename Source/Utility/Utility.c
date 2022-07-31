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
	printf("W:\tForward\n");
	printf("A:\tLeft\n");
	printf("S:\tBackward\n");
	printf("D:\tRight\n");
	printf("E:\tUp\n");
	printf("Q:\tDown\n");

	// End:
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
}

// Initialize Vulkan structs to default values:
void initialize_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanValidation *validation, FracRenderVulkanSwapchain *swapchain,
	FracRenderVulkanDescriptors *descriptors, FracRenderVulkanPipeline *pipeline,
	FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanCommands *commands,
	int sdf_type)
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

	descriptors->num_g_buffer_descriptors = 2;
	descriptors->g_buffer_descriptor_layout		= VK_NULL_HANDLE;
	descriptors->g_buffer_descriptors		= NULL;

	descriptors->sdf_3d_descriptor_layout		= VK_NULL_HANDLE;
	descriptors->sdf_3d_descriptor			= VK_NULL_HANDLE;
	descriptors->sdf_3d_buffer			= VK_NULL_HANDLE;
	descriptors->sdf_3d_memory			= VK_NULL_HANDLE;

	descriptors->sdf_2d_descriptor_layout		= VK_NULL_HANDLE;
	descriptors->sdf_2d_descriptor			= VK_NULL_HANDLE;

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

	if (sdf_type != 1)
	{
		pipeline->geometry_vertex_shader_path = "Assets/Shaders/Fractal-Geometry.vert.sprv";
	}

	if (sdf_type == 0)
	{
		pipeline->geometry_fragment_shader_path	=
			"Assets/Shaders/Fractal-Geometry-SDF-3D.frag.sprv";
	}
	else if (sdf_type == 1)
	{
		pipeline->geometry_vertex_shader_path =
			"Assets/Shaders/Fractal-Geometry-SDF-2D.vert.sprv";
		pipeline->geometry_fragment_shader_path	=
			"Assets/Shaders/Fractal-Geometry-SDF-2D.frag.sprv";
	}
	else
	{
		pipeline->geometry_fragment_shader_path	=
			"Assets/Shaders/Fractal-Geometry.frag.sprv";
	}

	pipeline->colour_vertex_shader_path	= "Assets/Shaders/Fractal-Colour.vert.sprv";
	pipeline->colour_fragment_shader_path	= "Assets/Shaders/Fractal-Colour.frag.sprv";

	// Framebuffers:
	framebuffers->framebuffers		= NULL;

	framebuffers->g_buffer			= VK_NULL_HANDLE;

	if (sdf_type == 1)
	{
		framebuffers->num_g_buffer_images = 3;
	}
	else
	{
		framebuffers->num_g_buffer_images = 2;
	}
	framebuffers->g_buffer_images		= NULL;
	framebuffers->g_buffer_image_views	= NULL;

	// Allocate memory for G-buffer formats (free in destroy_vulkan_framebuffers):
	framebuffers->g_buffer_formats		= malloc(framebuffers->num_g_buffer_images *
									sizeof(VkFormat));
	if (sdf_type == 1)
	{
		// Positions + iterations, normals and distance write:
		framebuffers->g_buffer_formats[0]	= VK_FORMAT_R32G32B32A32_SFLOAT;
		framebuffers->g_buffer_formats[1]	= VK_FORMAT_R32G32B32A32_SFLOAT;
		framebuffers->g_buffer_formats[2]	= VK_FORMAT_R32_SFLOAT;
	}
	else
	{
		// Positions + iterations and normals:
		framebuffers->g_buffer_formats[0]	= VK_FORMAT_R32G32B32A32_SFLOAT;
		framebuffers->g_buffer_formats[1]	= VK_FORMAT_R32G32B32A32_SFLOAT;
	}

	framebuffers->sdf_2d_image		= VK_NULL_HANDLE;
	framebuffers->sdf_2d_memory		= VK_NULL_HANDLE;
	framebuffers->sdf_2d_image_view		= VK_NULL_HANDLE;
	framebuffers->sdf_2d_format		= VK_FORMAT_R32_SFLOAT;

	// Commands:
	commands->command_pool		= VK_NULL_HANDLE;
	commands->command_buffers	= NULL;
	commands->fences		= NULL;
	commands->image_available	= VK_NULL_HANDLE;
	commands->render_finished	= VK_NULL_HANDLE;
}

// Destroy contents of Vulkan structs:
void destroy_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
		FracRenderVulkanSwapchain *swapchain, FracRenderVulkanDescriptors *descriptors,
		FracRenderVulkanPipeline *pipeline, FracRenderVulkanFramebuffers *framebuffers,
		FracRenderVulkanCommands *commands)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Cleaning up...\n");

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