#ifndef FRACRENDER_UTILITY_UTILITY_H
#define FRACRENDER_UTILITY_UTILITY_H

// Library includes:
#include <stdio.h>

// Local includes:
#include "../Vulkan/00-Vulkan-API.h"

/***********************
 * Function Prototypes *
 ***********************/

// Print title:
void print_title();

// Print fractal and SDF type:
void print_fractal_and_sdf_type(int fractal_type, int sdf_type);

// Print keyboard controls:
void print_controls();

// Initialize Vulkan structs to default values:
void initialize_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanValidation *validation, FracRenderVulkanSwapchain *swapchain,
	FracRenderVulkanDescriptors *descriptors, FracRenderVulkanPipeline *pipeline,
	FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanCommands *commands,
	int fractal_type, int sdf_type);

// Destroy contents of Vulkan structs:
void destroy_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
		FracRenderVulkanSwapchain *swapchain, FracRenderVulkanDescriptors *descriptors,
		FracRenderVulkanPipeline *pipeline, FracRenderVulkanFramebuffers *framebuffers,
		FracRenderVulkanCommands *commands);

#endif
