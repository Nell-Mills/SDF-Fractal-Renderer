#ifndef FRACRENDER_VULKAN_DEVICE_H
#define FRACRENDER_VULKAN_DEVICE_H

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"

// Data types:
typedef struct {
	int max_level;

	// Devices:
	VkPhysicalDevice physical_device;
	VkDevice logical_device;

	// Device extensions:
	uint32_t num_device_extensions;
	char *device_extensions[1];

	// Queue families:
	uint32_t graphics_family_index;
	VkQueue graphics_queue;
	uint32_t present_family_index;
	VkQueue present_queue;
} FracRenderVulkanDevice;

#endif
