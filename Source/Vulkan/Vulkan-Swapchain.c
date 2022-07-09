#include "Vulkan-Swapchain.h"

// Create Vulkan swapchain and associated images:
int initialize_vulkan_swapchain(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
							FracRenderVulkanSwapchain *swapchain)
{


	return 0;
}

// Destroy Vulkan swapchain structure:
void destroy_vulkan_swapchain(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain)
{
	// Destroy framebuffers:

	// Destroy swapchain image views:
	if (swapchain->swapchain_image_views)
	{
		for (uint32_t i = 0; i < swapchain->num_swapchain_images; i++)
		{
			vkDestroyImageView(device->logical_device,
				swapchain->swapchain_image_views[i], NULL);
		}
	}

	// Destroy swapchain:
	if (swapchain->swapchain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(device->logical_device, swapchain->swapchain, NULL);
	}
}

// Recreate Vulkan swapchain and associated images:
int recreate_vulkan_swapchain(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
							FracRenderVulkanSwapchain *swapchain)
{
	return 0;
}

// Create Vulkan swapchain:
int create_swapchain(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
					FracRenderVulkanSwapchain *swapchain)
{

}

// Create swapchain images:
int create_swapchain_images(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
					FracRenderVulkanSwapchain *swapchain)
{

}
