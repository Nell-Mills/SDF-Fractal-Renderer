#ifndef FRACRENDER_VULKAN_SWAPCHAIN_H
#define FRACRENDER_VULKAN_SWAPCHAIN_H

/********************************************************************
 * To set up and destroy the Vulkan swapchain and associated images *
 ********************************************************************/

// Library includes:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"
#include "01-Vulkan-Structs.h"

/***********************
 * Function Prototypes *
************************/

// Create Vulkan swapchain and associated images:
int initialize_vulkan_swapchain(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
							FracRenderVulkanSwapchain *swapchain);

// Destroy Vulkan swapchain structure:
void destroy_vulkan_swapchain(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain);

// Recreate Vulkan swapchain and associated images:
int recreate_vulkan_swapchain(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
							FracRenderVulkanSwapchain *swapchain);

// Create Vulkan swapchain:
int create_swapchain(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, VkSwapchainKHR old_swapchain);

// Create swapchain images and image views:
int create_swapchain_images(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain);

#endif
