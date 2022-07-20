#ifndef FRACRENDER_VULKAN_PIPELINE_H
#define FRACRENDER_VULKAN_PIPELINE_H

/***************************************************************
 * To set up and destroy the Vulkan pipeline and render passes *
 ***************************************************************/

// Library includes:
#include <stdio.h>
#include <stdlib.h>

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"
#include "01-Vulkan-Structs.h"

/***********************
 * Function Prototypes *
************************/

// Create Vulkan pipelines and render passes:
int initialize_vulkan_pipeline(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanDescriptors *descriptors,
	FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanPipeline *pipeline);

// Destroy Vulkan pipeline structure:
void destroy_vulkan_pipeline(FracRenderVulkanDevice *device, FracRenderVulkanPipeline *pipeline);

// Load shaders:
int load_shaders(FracRenderVulkanDevice *device, FracRenderVulkanPipeline *pipeline);

// Load shader module:
VkShaderModule load_shader_module(FracRenderVulkanDevice *device, const char *shader_path);

// Create geometry render pass:
int create_geometry_render_pass(FracRenderVulkanDevice *device, FracRenderVulkanPipeline *pipeline,
							FracRenderVulkanFramebuffers *framebuffers);

// Create colour render pass:
int create_colour_render_pass(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanPipeline *pipeline);

// Create pipeline layout:
int create_pipeline_layout(FracRenderVulkanDevice *device, FracRenderVulkanPipeline *pipeline,
					 FracRenderVulkanDescriptors *descriptors, int pipe);

// Create pipeline:
int create_pipeline(FracRenderVulkanDevice *device, FracRenderVulkanPipeline *pipeline,
					FracRenderVulkanSwapchain *swapchain, int pipe);

#endif
