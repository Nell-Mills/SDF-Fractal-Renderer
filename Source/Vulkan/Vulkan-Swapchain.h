#ifndef FRACRENDER_VULKAN_SWAPCHAIN_H
#define FRACRENDER_VULKAN_SWAPCHAIN_H

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"
#include "Vulkan-Structs.h"

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
					FracRenderVulkanSwapchain *swapchain);

// Create swapchain images:
int create_swapchain_images(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
					FracRenderVulkanSwapchain *swapchain);

#endif
