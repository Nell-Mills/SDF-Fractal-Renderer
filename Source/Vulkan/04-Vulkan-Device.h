#ifndef FRACRENDER_VULKAN_DEVICE_H
#define FRACRENDER_VULKAN_DEVICE_H

/*****************************************************************
 * To set up and destroy the Vulkan physical and logical devices *
 *****************************************************************/

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

// Initiaize Vulkan device struct:
int initalize_vulkan_device(FracRenderVulkanBase *base, FracRenderVulkanDevice *device);

// Destroy Vulkan device structure:
void destroy_vulkan_device(FracRenderVulkanDevice *device);

// Select physical device:
int select_physical_device(FracRenderVulkanBase *base, FracRenderVulkanDevice *device);

// Give a device a score based on which requirements it meets:
int score_physical_device(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
						VkPhysicalDevice physical_device);

// Create logical device:
int create_logical_device(FracRenderVulkanBase *base, FracRenderVulkanDevice *device);

// Get device queues:
int get_device_queues(FracRenderVulkanDevice *device);

#endif
