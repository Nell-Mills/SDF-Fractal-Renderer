#ifndef FRACRENDER_VULKAN_BASE_H
#define FRACRENDER_VULKAN_BASE_H

/*************************************************************************
 * To set up and destroy the Vulkan environment and window for rendering *
 *************************************************************************/

// Library includes:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"
#include "Vulkan-Structs.h"

/***********************
 * Function Prototypes *
************************/

// Initialize Vulkan environment and populate data structure:
int initialize_vulkan_base(FracRenderVulkanBase *base,
	FracRenderVulkanLayersExtensions *layers_extensions);

// Destroy Vulkan environment based on data structure:
int destroy_vulkan_base(FracRenderVulkanBase *base);

// Create GLFW window and KHR surface:
int create_glfw_window(FracRenderVulkanBase *base);

// Get supported instance extensions:
char **get_supported_instance_extensions(uint32_t *num_instance_extensions);

// Get supported instance layers:
char **get_supported_instance_layers(uint32_t *num_instance_layers);

// Create the Vulkan instance:
int create_vulkan_instance(FracRenderVulkanBase *base,
	FracRenderVulkanLayersExtensions *layers_extensions);

#endif
