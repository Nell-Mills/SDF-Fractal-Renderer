#ifndef FRACRENDER_VULKAN_SDF_H
#define FRACRENDER_VULKAN_SDF_H

/**********************************************************************
 * To set up and destroy the Vulkan descriptor and buffer for the SDF *
 **********************************************************************/

// Library includes:
#include <stdio.h>
#include <string.h>

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"
#include "01-Vulkan-Structs.h"
#include "../SDF/SDF.h"

/***********************
 * Function Prototypes *
 ***********************/

// Initialize SDF descriptor layout, descriptor and device memory:
int initialize_vulkan_sdf(FracRenderSDF *sdf, FracRenderVulkanSDF *sdf_vulkan,
	FracRenderVulkanDevice *device, FracRenderVulkanDescriptors *descriptors);

// Destroy SDF Vulkan structure:
void destroy_vulkan_sdf(FracRenderVulkanDevice *device, FracRenderVulkanSDF *sdf_vulkan);

// Create buffer for SDF:
int create_sdf_buffer(FracRenderSDF *sdf, FracRenderVulkanSDF *sdf_vulkan,
					FracRenderVulkanDevice *device);

// Create SDF descriptor set layout:
int create_sdf_descriptor_layout(FracRenderVulkanSDF *sdf_vulkan, FracRenderVulkanDevice *device);

// Create SDF descriptor:
int create_sdf_descriptor(FracRenderVulkanSDF *sdf_vulkan, FracRenderVulkanDevice *device,
						FracRenderVulkanDescriptors *descriptors);

// Copy data into the SDF buffer:
int copy_sdf_data(FracRenderSDF *sdf, FracRenderVulkanSDF *sdf_vulkan,
	FracRenderVulkanDevice *device, FracRenderVulkanCommands *commands);

#endif
