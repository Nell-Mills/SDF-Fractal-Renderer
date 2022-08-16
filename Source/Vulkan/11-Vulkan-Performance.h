#ifndef FRACRENDER_VULKAN_PERFORMANCE_H
#define FRACRENDER_VULKAN_PERFORMANCE_H

/*************************************************************
 * To set up and destroy the Vulkan performance measurements *
 *************************************************************/

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
void get_shader_time(double *shader_time, int num_frames, uint32_t image_index, int order,
		FracRenderVulkanDevice *device, FracRenderVulkanPerformance *performance);

// Write measurements to file:
void write_measurements(FILE *performance_file, double **multi_shader_time, int num_frames,
									int max_values);

// Sort elements of array in ascending order:
void sort_array_ascending(double *array, int num_elements);

#endif
