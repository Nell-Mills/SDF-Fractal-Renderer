#ifndef FRACRENDER_VULKAN_MAIN_H
#define FRACRENDER_VULKAN_MAIN_H

/***************************************
 * Main Vulkan functions for rendering *
 ***************************************/

// Library includes:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"
#include "01-Vulkan-Structs.h"
#include "../Utility/Program-State.h"
#include "../Utility/Vectors.h"

/***********************
 * Function Prototypes *
************************/

void update_scene_uniform(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanSceneUniform *scene_uniform,
	FracRenderProgramState *program_state);

// Record commands:
int record_commands(FracRenderVulkanSwapchain *swapchain, FracRenderVulkanDescriptors *descriptors,
		FracRenderVulkanPipeline *pipeline, FracRenderVulkanFramebuffers *framebuffers,
		FracRenderVulkanCommands *commands, FracRenderVulkanPerformance *performance,
		FracRenderVulkanSceneUniform *scene_uniform, FracRenderProgramState *program_state,
		uint32_t image_index);

// Submit commands:
int submit_commands(FracRenderVulkanDevice *device, FracRenderVulkanCommands *commands,
								uint32_t image_index);

// Present results:
int present_results(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
				FracRenderVulkanCommands *commands, uint32_t image_index);

// Copy contents of G-Buffer distance image to texture:
int copy_g_buffer_image(FracRenderVulkanSwapchain *swapchain,
	FracRenderVulkanFramebuffers *framebuffers, VkCommandBuffer command_buffer);

#endif
