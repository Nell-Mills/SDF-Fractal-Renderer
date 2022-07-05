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

// Initialize Vulkan structs to default values:
void initialize_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanValidation *validation, FracRenderVulkanSwapchain *swapchain);

// Destroy contents of Vulkan structs:
void destroy_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanValidation *validation, FracRenderVulkanSwapchain *swapchain, int level);

// Print title:
void print_title();

// Print keyboard and mouse controls:
void print_controls();

/********
 * Main *
 ********/
int main(int argc, char **argv)
{
	int level = 0;

	// Print title:
	print_title();

	// Print keyboard controls:
	print_controls();

	// Initialize the Vulkan structs:
	FracRenderVulkanBase base;
	FracRenderVulkanDevice device;
	FracRenderVulkanValidation validation;
	FracRenderVulkanSwapchain swapchain;
	initialize_vulkan_structs(&base, &device, &validation, &swapchain);

	// Check support for required layers and extensions:
	if (check_layer_support() != 0)
	{
		fprintf(stderr, "Error: Required layers not supported!\n");
		destroy_vulkan_structs(&base, &device, &validation, &swapchain, level);
		return -1;
	}
	if (check_extension_support() != 0)
	{
		fprintf(stderr, "Error: Required extensions not supported!\n");
		destroy_vulkan_structs(&base, &device, &validation, &swapchain, level);
		return -1;
	}

	// Initialize the Vulkan base (instance, window and surface):
	if (initialize_vulkan_base(&base, &validation) != 0)
	{
		destroy_vulkan_structs(&base, &device, &validation, &swapchain, level);
		return -1;
	}

	return 0;
}

/*************
 * Utilities *
 *************/

// Initialize Vulkan structs to default values:
void initialize_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanValidation *validation, FracRenderVulkanSwapchain *swapchain)
{
	// Vulkan base:
	base->max_level			= 3;
	base->vulkan_instance		= VK_NULL_HANDLE;
	base->window			= NULL;
	base->present_surface		= VK_NULL_HANDLE;

	// Device:
	device->max_level		= 0;

	device->physical_device		= VK_NULL_HANDLE,
	device->logical_device		= VK_NULL_HANDLE,

	device->num_device_extensions		= 1;
	device->device_extensions[0]		= "VK_KHR_SWAPCHAIN_EXTENSION_NAME";

	device->graphics_family_index		= 0;
	device->graphics_queue			= VK_NULL_HANDLE;
	device->present_family_index		= 0;
	device->present_queue			= VK_NULL_HANDLE;

	// Validation:
	validation->max_level			= 0;

	validation->enable_validation		= ENABLE_VALIDATION;
	validation->num_validation_layers	= 1;
	validation->validation_layers[0]	= "VK_LAYER_KHRONOS_VALIDATION";

	validation->debug_messenger		= VK_NULL_HANDLE;

	// Swapchain:
	swapchain->max_level			= 0;

	swapchain->swapchain			= VK_NULL_HANDLE;
	swapchain->num_swapchain_images		= 0;
	swapchain->swapchain_images		= NULL;
	swapchain->swapchain_image_views	= NULL;
	swapchain->swapchain_format		= VK_FORMAT_UNDEFINED;
	swapchain->swapchain_extent.width	= 0;
	swapchain->swapchain_extent.height	= 0;
}

// Destroy contents of Vulkan structs:
void destroy_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanValidation *validation, FracRenderVulkanSwapchain *swapchain, int level)
{
	if (level > 1)
	{
		// Destroy Vulkan device:
	}
	if (level > 0)
	{
		// Destroy Vulkan base:
		destroy_vulkan_base(base, base->max_level);
	}
}

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
	printf("F11:\tSwitch between SDF on and off\n");

	// End:
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
}
