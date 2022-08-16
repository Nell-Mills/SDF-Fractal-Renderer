#ifndef FRACRENDER_UTILITY_UTILITY_H
#define FRACRENDER_UTILITY_UTILITY_H

/*******************************************************************************
 * Utilities for setting up and displaying program info, and setting up Vulkan *
 *******************************************************************************/

// Library includes:
#include <stdio.h>

// Local includes:
#include "../Vulkan/00-Vulkan-API.h"
#include "Animation.h"
#include "Input.h"
#include "Mandelbrot-Iterations.h"
#include "Program-State.h"
#include "Vectors.h"

/***********************
 * Function Prototypes *
 ***********************/

// Print title:
void print_title();

// Print fractal and SDF type:
void print_fractal_and_optimization_type(FracRenderProgramState *program_state);

// Print keyboard controls:
void print_controls();

// Set up program state according to program inputs:
void set_up_program_state(int argc, char **argv, FracRenderProgramState *program_state);

// Set up scene uniform object:
void set_up_scene_uniform(FracRenderProgramState *program_state, FracRenderSDF3D *sdf_3d,
					FracRenderVulkanSceneUniform *scene_uniform);

// Initialize Vulkan structs to default values:
void initialize_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanValidation *validation, FracRenderVulkanSwapchain *swapchain,
	FracRenderVulkanDescriptors *descriptors, FracRenderVulkanPipeline *pipeline,
	FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanCommands *commands,
	FracRenderVulkanPerformance *performance, FracRenderProgramState *program_state);

// Perform all setup of Vulkan environment and SDF (if any):
int set_up_vulkan(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
		FracRenderVulkanValidation *validation, FracRenderVulkanSwapchain *swapchain,
		FracRenderVulkanDescriptors *descriptors, FracRenderVulkanPipeline *pipeline,
		FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanCommands *commands,
		FracRenderVulkanPerformance *performance, FracRenderProgramState *program_state,
		FracRenderSDF3D *sdf_3d);

// Destroy contents of Vulkan structs:
void destroy_vulkan_structs(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
		FracRenderVulkanSwapchain *swapchain, FracRenderVulkanDescriptors *descriptors,
		FracRenderVulkanPipeline *pipeline, FracRenderVulkanFramebuffers *framebuffers,
		FracRenderVulkanCommands *commands, FracRenderVulkanPerformance *performance);

// Print all Vulkan handles for debugging:
void print_vulkan_handles(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
		FracRenderVulkanValidation *validation, FracRenderVulkanSwapchain *swapchain,
		FracRenderVulkanDescriptors *descriptors, FracRenderVulkanPipeline *pipeline,
		FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanCommands *commands,
		FracRenderVulkanPerformance *performance);

#endif
