#ifndef FRACRENDER_VULKAN_DESCRIPTORS_H
#define FRACRENDER_VULKAN_DESCRIPTORS_H

/************************************************
 * To set up and destroy the Vulkan descriptors *
 ************************************************/

// Library includes:
#include <stdio.h>
#include <stdlib.h>

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"
#include "01-Vulkan-Structs.h"

/***********************
 * Function Prototypes *
************************/

// Create Vulkan descriptors:
int initialize_vulkan_descriptors(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors);

// Destroy Vulkan descriptors:
void destroy_vulkan_descriptors(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors);

#endif
