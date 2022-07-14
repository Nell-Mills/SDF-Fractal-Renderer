#ifndef FRACRENDER_VULKAN_FRAMEBUFFERS_H
#define FRACRENDER_VULKAN_FRAMEBUFFERS_H

/*************************************************
 * To set up and destroy the Vulkan framebuffers *
 *************************************************/

// Library includes:
#include <stdio.h>

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"
#include "01-Vulkan-Structs.h"

/***********************
 * Function Prototypes *
************************/

// Create Vulkan framebuffers:
int initialize_vulkan_framebuffers();

// Destroy Vulkan framebuffer structure:
void destroy_vulkan_framebuffers(FracRenderVulkanDevice *device,
		FracRenderVulkanFramebuffers *framebuffers);

#endif
