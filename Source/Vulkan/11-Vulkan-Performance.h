#ifndef FRACRENDER_VULKAN_PERFORMANCE_H
#define FRACRENDER_VULKAN_PERFORMANCE_H

/*************************************************************
 * To set up and destroy the Vulkan performance measurements *
 *************************************************************/

// Library includes:
#include <stdio.h>
#include <string.h>

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"
#include "01-Vulkan-Structs.h"

/***********************
 * Function Prototypes *
************************/

// Create Vulkan performance structure:
int initialize_vulkan_performance(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanPerformance *performance);

// Destroy Vulkan performance structure:
void destroy_vulkan_performance(FracRenderVulkanDevice *device,
			FracRenderVulkanPerformance *performance);

// Query timestamp support:
int query_timestamp_support(FracRenderVulkanDevice *device,
		FracRenderVulkanPerformance *performance);

// Create query pool:
int create_query_pool(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
						FracRenderVulkanPerformance *performance);

// Get difference between 2 timestamps:
void get_shader_time(double *shader_time, int num_elements, uint32_t image_index,
	FracRenderVulkanDevice *device, FracRenderVulkanPerformance *performance);

#endif
