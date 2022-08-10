#include "11-Vulkan-Performance.h"

// Create Vulkan performance structure:
int initialize_vulkan_performance(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanPerformance *performance)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Initializing Vulkan timestamp query pool...\n");

	// See if timestamps are supported:
	printf(" ---> Querying timestamp support.\n");
	if (query_timestamp_support(device, performance) != 0)
	{
		return -1;
	}

	// Create query pool:
	printf(" ---> Creating timestamp query pool.\n");
	if (create_query_pool(device, swapchain, performance) != 0)
	{
		return -1;
	}

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	return 0;
}

// Destroy Vulkan performance structure:
void destroy_vulkan_performance(FracRenderVulkanDevice *device,
			FracRenderVulkanPerformance *performance)
{
	printf(" ---> Destroying Vulkan timestamp query pool.\n");

	if (performance->query_pool != VK_NULL_HANDLE)
	{
		vkDestroyQueryPool(device->logical_device, performance->query_pool, NULL);
	}
}

// Query timestamp support:
int query_timestamp_support(FracRenderVulkanDevice *device,
		FracRenderVulkanPerformance *performance)
{
	// Get physical device properties:
	VkPhysicalDeviceProperties physical_device_properties;
	vkGetPhysicalDeviceProperties(device->physical_device, &physical_device_properties);

	// Query timestamp support:
	if (physical_device_properties.limits.timestampComputeAndGraphics != VK_TRUE)
	{
		fprintf(stderr, "Error: Timestamps not supported!\n");
		return -1;
	}

	// Get timestamp period:
	performance->timestamp_period = physical_device_properties.limits.timestampPeriod;

	return 0;
}

// Create query pool:
int create_query_pool(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
						FracRenderVulkanPerformance *performance)
{
	// Define pool creation info:
	VkQueryPoolCreateInfo pool_info;
	memset(&pool_info, 0, sizeof(VkQueryPoolCreateInfo));
	pool_info.sType			= VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
	pool_info.pNext			= NULL;
	pool_info.flags			= 0;
	pool_info.queryType		= VK_QUERY_TYPE_TIMESTAMP;
	pool_info.queryCount		= 2 * swapchain->num_swapchain_images;
	pool_info.pipelineStatistics	= 0;

	// Create the pool:
	if (vkCreateQueryPool(device->logical_device, &pool_info,
		NULL, &performance->query_pool) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to create timestamp query pool!\n");
		return -1;
	}
}

// Get difference between 2 timestamps:
void get_shader_time(double *shader_time, int num_elements, uint32_t image_index,
	FracRenderVulkanDevice *device, FracRenderVulkanPerformance *performance)
{
	// Get values in timestamp queries. Get 64-bit values and wait for availability:
	uint64_t timestamps[2];
	VkResult result = vkGetQueryPoolResults(
		device->logical_device,
		performance->query_pool,
		2 * image_index,	// First query.
		2,			// Query count.
		2 * sizeof(uint64_t),
		timestamps,
		sizeof(uint64_t),
		VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT
	);

	// Get difference between timestamps:
	uint64_t time = timestamps[1] - timestamps[0];
	double real_time = (double)(time) * performance->timestamp_period;

	// Insert into the array in order:
	int i = 0;
	for (; i < num_elements; i++)
	{
		if (shader_time[i] >= real_time) { break; }
	}

	// Move other elements along:
	if (i == num_elements) { shader_time[i - 1] = real_time; }
	else
	{
		for (int j = (num_elements - 1); j >= (i + 1); j--)
		{
			shader_time[j] = shader_time[j - 1];
		}
		shader_time[i] = real_time;
	}
}
