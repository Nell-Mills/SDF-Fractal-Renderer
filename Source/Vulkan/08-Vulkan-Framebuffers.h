#ifndef FRACRENDER_VULKAN_FRAMEBUFFERS_H
#define FRACRENDER_VULKAN_FRAMEBUFFERS_H

/*************************************************
 * To set up and destroy the Vulkan framebuffers *
 *************************************************/

// Library includes:
#include <stdio.h>
#include <stdlib.h>

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"
#include "01-Vulkan-Structs.h"

/***********************
 * Function Prototypes *
************************/

// Create Vulkan framebuffers:
int initialize_vulkan_framebuffers(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanPipeline *pipeline,
	FracRenderVulkanFramebuffers *framebuffers);

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
	FracRenderVulkanPipeline *pipeline, FracRenderVulkanFramebuffers *framebuffers);

// Create sampler:
int create_sampler(FracRenderVulkanDevice *device, FracRenderVulkanFramebuffers *framebuffers);

#endif
