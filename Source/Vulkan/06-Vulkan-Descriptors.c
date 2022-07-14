#include "06-Vulkan-Descriptors.h"

// Create Vulkan descriptors:
int initialize_vulkan_descriptors(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Initializing Vulkan descriptors...\n");

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	return 0;
}

// Destroy Vulkan descriptors:
void destroy_vulkan_descriptors(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors)
{
	printf(" ---> Destroying Vulkan descriptors.\n");

	// Destroy scene descriptor:
	if (descriptors->scene_descriptor != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorSetLayout(device->logical_device,
				descriptors->scene_descriptor, NULL);
	}
}
