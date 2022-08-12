#ifndef FRACRENDER_VULKAN_FRAMEBUFFERS_H
#define FRACRENDER_VULKAN_FRAMEBUFFERS_H

/*************************************************
 * To set up and destroy the Vulkan framebuffers *
 *************************************************/

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

// Create Vulkan framebuffers:
int initialize_vulkan_framebuffers(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanPipeline *pipeline,
	FracRenderVulkanFramebuffers *framebuffers, int optimize);

// Destroy Vulkan framebuffer structure:
void destroy_vulkan_framebuffers(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanFramebuffers *framebuffers);

// Create framebuffers for the swapchain:
int create_swapchain_framebuffers(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanPipeline *pipeline,
	FracRenderVulkanFramebuffers *framebuffers);

// Create G-buffer images and image views:
int create_g_buffer_images(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
						FracRenderVulkanFramebuffers *framebuffers);

// Create G-buffer:
int create_g_buffer(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
	FracRenderVulkanPipeline *pipeline, FracRenderVulkanFramebuffers *framebuffers,
	int optimize);

// Create Temporal Cache image and image view:
int create_temporal_cache_image(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanFramebuffers *framebuffers);

// Initialize Temporal Cache image to zero:
int initialize_temporal_cache_image(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanFramebuffers *framebuffers,
	FracRenderVulkanCommands *commands);

// Recreate swapchain framebuffers:
int recreate_vulkan_swapchain_framebuffers(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanPipeline *pipeline,
	FracRenderVulkanFramebuffers *framebuffers, uint32_t num_images);

// Recreate G-buffer images and views:
int recreate_vulkan_g_buffer_images(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanFramebuffers *framebuffers);

// Recreate G-buffer:
int recreate_vulkan_g_buffer(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
	FracRenderVulkanPipeline *pipeline, FracRenderVulkanFramebuffers *framebuffers,
	int optimize);

// Recreate Temporal Cache image and image view:
int recreate_temporal_cache_image(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanFramebuffers *framebuffers);

#endif
