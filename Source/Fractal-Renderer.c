/********************
 * Fractal Renderer *
 ********************/

// Library includes:
#include <stdio.h>

// Local includes:
#include "Vulkan/Vulkan-API.h"

/***********************
 * Function Prototypes *
 ***********************/

// Print title:
void print_title();

// Print keyboard and mouse controls:
void print_controls();

// Initialize Vulkan structs to default values:
void initialize_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanLayersExtensions *layers_extensions, FracRenderVulkanSwapchain *swapchain);

// Destroy contents of Vulkan structs:
int destroy_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanLayersExtensions *layers_extensions, FracRenderVulkanSwapchain *swapchain);

/********
 * Main *
 ********/

int main(int argc, char **argv)
{
	// Initialize Volk:
	if (initialize_volk() != 0)
	{
		return -1;
	}

	// Initialize the Vulkan structs:
	FracRenderVulkanBase base;
	FracRenderVulkanDevice device;
	FracRenderVulkanLayersExtensions layers_extensions;
	FracRenderVulkanSwapchain swapchain;
	initialize_vulkan_structs(&base, &device, &layers_extensions, &swapchain);

	// Check support for required validation layers:
	if (check_validation_support(&layers_extensions) != 0)
	{
		destroy_vulkan_structs(&base, &device, &layers_extensions, &swapchain);
		return -1;
	}

	// Initialize the Vulkan base (instance, window and surface):
	if (initialize_vulkan_base(&base, &layers_extensions) != 0)
	{
		destroy_vulkan_structs(&base, &device, &layers_extensions, &swapchain);
		return -1;
	}

	// Print title:
	print_title();

	// Print keyboard controls:
	print_controls();

	// Destroy Vulkan structs:
	destroy_vulkan_structs(&base, &device, &layers_extensions, &swapchain);

	return 0;
}

/*************
 * Utilities *
 *************/

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

// Print keyboard and mouse controls:
void print_controls()
{
	// Title:
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Controls:\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n");

	// Keyboard controls:
	printf("Home:\tPrint controls\n");
	printf("F11:\tSwitch SDF on and off\n");

	// End:
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
}

// Initialize Vulkan structs to default values:
void initialize_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanLayersExtensions *layers_extensions, FracRenderVulkanSwapchain *swapchain)
{
	// Vulkan base:
	base->initialization_level	= 0;
	base->vulkan_instance		= VK_NULL_HANDLE;
	base->window			= NULL;
	base->present_surface		= VK_NULL_HANDLE;

	base->debug_messenger		= VK_NULL_HANDLE;

	// Device:
	device->initialization_level	= 0;

	device->physical_device		= VK_NULL_HANDLE,
	device->logical_device		= VK_NULL_HANDLE,

	device->num_device_extensions	= 1;
	device->device_extensions[0]	= VK_KHR_SWAPCHAIN_EXTENSION_NAME;

	device->graphics_family_index	= 0;
	device->graphics_queue		= VK_NULL_HANDLE;
	device->present_family_index	= 0;
	device->present_queue		= VK_NULL_HANDLE;

	// Layers and extensions:
#ifdef FRACRENDER_DEBUG
	layers_extensions->num_validation_layers	= 1;
	layers_extensions->num_validation_extensions	= 1;
	layers_extensions->validation_layers[0]		= "VK_LAYER_KHRONOS_validation";
	layers_extensions->validation_extensions[0]	= "VK_EXT_debug_utils";
#else
	layers_extensions->num_validation_layers	= 0;
	layers_extensions->num_validation_extensions	= 0;
#endif

	layers_extensions->num_glfw_extensions		= 0;
	layers_extensions->glfw_extensions		= glfwGetRequiredInstanceExtensions(
							&layers_extensions->num_glfw_extensions);

	// Swapchain:
	swapchain->initialization_level		= 0;

	swapchain->swapchain			= VK_NULL_HANDLE;
	swapchain->num_swapchain_images		= 0;
	swapchain->swapchain_images		= NULL;
	swapchain->swapchain_image_views	= NULL;
	swapchain->swapchain_format		= VK_FORMAT_UNDEFINED;
	swapchain->swapchain_extent.width	= 0;
	swapchain->swapchain_extent.height	= 0;
}

// Destroy contents of Vulkan structs:
int destroy_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanLayersExtensions *layers_extensions, FracRenderVulkanSwapchain *swapchain)
{
	// Destroy Vulkan base:
	destroy_vulkan_base(base);

	return 0;
}
