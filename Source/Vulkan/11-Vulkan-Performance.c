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
void get_shader_time(double *shader_time, int num_frames, uint32_t image_index, int order,
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

	// Get difference between render pass timestamps:
	uint64_t render_pass_time = timestamps[1] - timestamps[0];
	double real_render_pass_time = (double)(render_pass_time) * performance->timestamp_period;

	if (order == 0)
	{
		// Insert into the array in order:
		int i = 0;
		for (; i < num_frames; i++)
		{
			if (shader_time[i] >= real_render_pass_time) { break; }
		}

		// Move other elements along:
		if (i == num_frames) { shader_time[i - 1] = real_render_pass_time; }
		else
		{
			for (int j = (num_frames - 1); j >= (i + 1); j--)
			{
				shader_time[j] = shader_time[j - 1];
			}
			shader_time[i] = real_render_pass_time;
		}
	}
	else
	{
		// Insert into free place:
		shader_time[num_frames - 1] = real_render_pass_time;
	}
}

// Write measurements to file:
void write_measurements(FILE *performance_file, double **multi_shader_time, int num_frames,
									int max_values)
{
	// Gather nth frames from each animation runthrough and write to file:
	double *frame = malloc(max_values * sizeof(double));
	for (int i = 0; i < num_frames; i++)
	{
		// Gather ith frame:
		for (int j = 0; j < max_values; j++)
		{
			frame[j] = multi_shader_time[j][i];
		}

		// Sort array ascending:
		sort_array_ascending(frame, max_values);

		// Take median, min and max:
		fprintf(performance_file, "%d\t", i);
		fprintf(performance_file, "%.1lf\t", frame[max_values / 2]);
		fprintf(performance_file, "%.1lf\t", frame[0]);
		fprintf(performance_file, "%.1lf\n", frame[max_values - 1]);
	}

	// Free memory:
	free(frame);
}

// Sort elements of array in ascending order:
void sort_array_ascending(double *array, int num_elements)
{
	// Create new array, copy data and reset contents of original array to 0:
	double *new_array = malloc(num_elements * sizeof(double));
	memcpy(new_array, array, num_elements * sizeof(double));
	memset(array, 0, num_elements * sizeof(double));

	// Insert all elements back into original array according to value:
	for (int i = 0; i < num_elements; i++)
	{
		int j = 0;
		for (; j < (i + 1); j++)
		{
			if (array[j] >= new_array[i]) { break; }
		}

		if (j == (i + 1)) { array[j - 1] = new_array[i]; }
		else
		{
			for (int k = i; k >= (j + 1); k--)
			{
				array[k] = array[k - 1];
			}
			array[j] = new_array[i];
		}
	}

	// Free memory:
	free(new_array);
}
