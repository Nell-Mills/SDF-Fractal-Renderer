#ifndef FRACRENDER_VULKAN_SWAPCHAIN_H
#define FRACRENDER_VULKAN_SWAPCHAIN_H

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"

// Data types:
typedef struct {
	int max_level;

	// Swapchain:
	VkSwapchainKHR swapchain;

	// Images:
	uint32_t num_swapchain_images;
	VkImage *swapchain_images;
	VkImageView *swapchain_image_views;

	// Format:
	VkFormat swapchain_format;
	VkExtent2D swapchain_extent;
} FracRenderVulkanSwapchain;

#endif
