#ifndef FRACRENDER_UTILITY_UTILITY_H
#define FRACRENDER_UTILITY_UTILITY_H

/********************************************************************************************
 * Utilities for printing program info and initializing Vulkan structures to default values *
 ********************************************************************************************/

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
void print_fractal_and_sdf_type(FracRenderProgramState *program_state);

// Print keyboard controls:
void print_controls();

// Initialize Vulkan structs to default values:
void initialize_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanValidation *validation, FracRenderVulkanSwapchain *swapchain,
	FracRenderVulkanDescriptors *descriptors, FracRenderVulkanPipeline *pipeline,
	FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanCommands *commands,
	FracRenderVulkanPerformance *performance, FracRenderProgramState *program_state);

// Destroy contents of Vulkan structs:
void destroy_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
		FracRenderVulkanSwapchain *swapchain, FracRenderVulkanDescriptors *descriptors,
		FracRenderVulkanPipeline *pipeline, FracRenderVulkanFramebuffers *framebuffers,
		FracRenderVulkanCommands *commands, FracRenderVulkanPerformance *performance);

#endif
