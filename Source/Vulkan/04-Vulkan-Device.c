#include "04-Vulkan-Device.h"

// Select physical device and create logical device:
int initalize_vulkan_device(FracRenderVulkanBase *base, FracRenderVulkanDevice *device)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Initializing Vulkan device...\n");

	// Select physical device:
	printf(" ---> Selecting physical device.\n");
	if (select_physical_device(base, device) != 0)
	{
		return -1;
	}

	// Print physical device information:
	VkPhysicalDeviceProperties physical_device_properties;
	vkGetPhysicalDeviceProperties(device->physical_device, &physical_device_properties);
	printf("      - Device selected: %s (%d.%d.%d)\n",
		physical_device_properties.deviceName,
		VK_API_VERSION_MAJOR(physical_device_properties.apiVersion),
		VK_API_VERSION_MINOR(physical_device_properties.apiVersion),
		VK_API_VERSION_PATCH(physical_device_properties.apiVersion)
	);

	// Print supported device extensions:
	printf("      - Supported device extensions:\n");
	for (uint32_t i = 0; i < device->num_device_extensions; i++)
	{
		printf("        ---> %s\n", device->device_extensions[i]);
	}

	// Create logical device:
	printf(" ---> Creating logical device.\n");
	if (create_logical_device(base, device) != 0)
	{
		return -1;
	}

	// Get device queues:
	printf(" ---> Getting device queues.\n");
	if (get_device_queues(device) != 0)
	{
		return -1;
	}

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	return 0;
}

// Destroy Vulkan device structure:
void destroy_vulkan_device(FracRenderVulkanDevice *device)
{
	printf(" ---> Destroying Vulkan device.\n");

	if (device->logical_device != VK_NULL_HANDLE)
	{
		vkDestroyDevice(device->logical_device, NULL);
	}
}

// Select physical device:
int select_physical_device(FracRenderVulkanBase *base, FracRenderVulkanDevice *device)
{
	// Get available physical devices:
	uint32_t num_physical_devices = 0;
	if (vkEnumeratePhysicalDevices(base->instance, &num_physical_devices, NULL) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to enumerate physical devices!\n");
		return -1;
	}

	VkPhysicalDevice *physical_devices = malloc(num_physical_devices *
						sizeof(VkPhysicalDevice));
	if (vkEnumeratePhysicalDevices(base->instance, &num_physical_devices,
					physical_devices) != VK_SUCCESS)
	{
		// Free memory:
		free(physical_devices);

		fprintf(stderr, "Error: Unable to get physical devices!\n");
		return -1;
	}

	// Score the physical devices and select the best one:
	int best_score = -1;

	for (uint32_t i = 0; i < num_physical_devices; i++)
	{
		int score = score_physical_device(base, device, physical_devices[i]);
		if (score > best_score)
		{
			best_score = score;
			device->physical_device = physical_devices[i];
		}
	}

	// See if a device was selected:
	if (best_score < 0)
	{
		// Free memory:
		free(physical_devices);

		fprintf(stderr, "Error: No suitable physical device found!\n");
		return -1;
	}

	// Free memory:
	free(physical_devices);

	return 0;
}

// Give a device a score based on which requirements it meets:
int score_physical_device(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
						VkPhysicalDevice physical_device)
{
	int score = 0;

	// Get physical device properties:
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(physical_device, &properties);

	// Get supported Vulkan version:
	uint32_t version_major = VK_API_VERSION_MAJOR(properties.apiVersion);
	uint32_t version_minor = VK_API_VERSION_MINOR(properties.apiVersion);

	// If version is below 1.1, give device a negative score:
	if ((version_major < 1) || ((version_major == 1) && (version_minor < 1)))
	{
		return -1;
	}

	// See if device is discrete or integrated:
	if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		// Prefer discrete:
		score += 1;
	}

	// See if device supports required extensions:
	uint32_t num_supported_extensions = 0;
	if (vkEnumerateDeviceExtensionProperties(physical_device, NULL,
			&num_supported_extensions, NULL) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to enumerate physical device extensions!\n");
		return -1;
	}

	VkExtensionProperties *supported_extensions = malloc(num_supported_extensions
							* sizeof(VkExtensionProperties));
	if (vkEnumerateDeviceExtensionProperties(physical_device, NULL,
		&num_supported_extensions, supported_extensions) != VK_SUCCESS)
	{
		// Free memory:
		free(supported_extensions);

		fprintf(stderr, "Error: Failed to get physical device extensions!\n");
		return -1;
	}

	for (uint32_t i = 0; i < device->num_device_extensions; i++)
	{
		int supported = -1;
		for (uint32_t j = 0; j < num_supported_extensions; j++)
		{
			if (strcmp(device->device_extensions[i],
				supported_extensions[j].extensionName) == 0)
			{
				supported = 0;
			}
		}
		if (supported != 0)
		{
			// Free memory:
			free(supported_extensions);

			return -1;
		}
	}

	// Free memory:
	free(supported_extensions);

	// Check queue families for the device:
	int graphics_queue_family_found = -1;
	int surface_queue_family_found = -1;
	VkQueueFlags graphics_flags = VK_QUEUE_GRAPHICS_BIT;

	uint32_t num_queues = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &num_queues, NULL);
	VkQueueFamilyProperties *queues = malloc(num_queues * sizeof(VkQueueFamilyProperties));
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &num_queues, queues);

	for (uint32_t i = 0; i < num_queues; i++)
	{
		if ((graphics_queue_family_found == 0) && (surface_queue_family_found == 0))
		{
			break;
		}

		// Check for graphics queue:
		if (graphics_queue_family_found != 0)
		{
			if ((graphics_flags & queues[i].queueFlags) == graphics_flags)
			{
				device->graphics_family_index = i;
				graphics_queue_family_found = 0;
			}
		}

		// Check for surface queue:
		if (surface_queue_family_found != 0)
		{
			VkBool32 surface_supported = VK_FALSE;
			if (vkGetPhysicalDeviceSurfaceSupportKHR(physical_device,
				i, base->surface, &surface_supported) != VK_SUCCESS)
			{
				// Skip this queue:
				continue;
			}
			if (surface_supported == VK_TRUE)
			{
				device->present_family_index = i;
				surface_queue_family_found = 0;
			}
		}
	}

	// Check that both requirements are met:
	if ((graphics_queue_family_found != 0) || (surface_queue_family_found != 0))
	{
		// Free memory:
		free(queues);
		return -1;
	}

	// Free memory:
	free(queues);

	// Check supported device features (need shaderFloat64):
	VkPhysicalDeviceFeatures supported_features;
	vkGetPhysicalDeviceFeatures(physical_device, &supported_features);

	if (supported_features.shaderFloat64 != VK_TRUE)
	{
		return -1;
	}

	return score;
}

// Create logical device:
int create_logical_device(FracRenderVulkanBase *base, FracRenderVulkanDevice *device)
{
	// Create device queues:
	uint32_t num_queues;
	if (device->graphics_family_index == device->present_family_index)
	{
		// Only need one queue:
		num_queues = 1;
	}
	else
	{
		num_queues = 2;
	}

	VkDeviceQueueCreateInfo *queue_info = malloc(num_queues * sizeof(VkDeviceQueueCreateInfo));
	float queue_priorities[1] = { 1.f };

	for (uint32_t i = 0; i < num_queues; i++)
	{
		VkDeviceQueueCreateInfo *info = &queue_info[i];
		memset(info, 0, sizeof(VkDeviceQueueCreateInfo));
		info->sType		= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		info->pNext		= NULL;
		info->flags		= 0;
		info->queueCount	= 1;
		info->pQueuePriorities	= queue_priorities;

		if (i == 0)
		{
			info->queueFamilyIndex = device->graphics_family_index;
		}
		else
		{
			info->queueFamilyIndex = device->present_family_index;
		}
	}

	// Define enabled features (shaderFloat64):
	VkPhysicalDeviceFeatures enabled_features;
	memset(&enabled_features, 0, sizeof(VkPhysicalDeviceFeatures));
	enabled_features.shaderFloat64 = VK_TRUE;

	// Define the logical device creation info:
	VkDeviceCreateInfo logical_device_info;
	memset(&logical_device_info, 0, sizeof(VkDeviceCreateInfo));
	logical_device_info.sType			= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	logical_device_info.pNext			= NULL;
	logical_device_info.flags			= 0;
	logical_device_info.queueCreateInfoCount	= num_queues;
	logical_device_info.pQueueCreateInfos		= queue_info;
	logical_device_info.enabledLayerCount		= 0;
	logical_device_info.ppEnabledLayerNames		= NULL;
	logical_device_info.enabledExtensionCount	= device->num_device_extensions;
	logical_device_info.ppEnabledExtensionNames	= (const char **)device->device_extensions;
	logical_device_info.pEnabledFeatures		= &enabled_features;

	// Create the logical device:
	if (vkCreateDevice(device->physical_device, &logical_device_info, NULL,
					&device->logical_device) != VK_SUCCESS)
	{
		// Free memory:
		free(queue_info);

		fprintf(stderr, "Error: Unable to create logical device!\n");
		return -1;
	}

	// Free memory:
	free(queue_info);

	return 0;
}

// Get device queues:
int get_device_queues(FracRenderVulkanDevice *device)
{
	// Get graphics queue:
	vkGetDeviceQueue(device->logical_device, device->graphics_family_index,
						0, &device->graphics_queue);

	if (device->graphics_queue == VK_NULL_HANDLE)
	{
		fprintf(stderr, "Error: Unable to retrieve graphics queue!\n");
		return -1;
	}

	// Get present queue:
	if (device->graphics_family_index == device->present_family_index)
	{
		device->present_queue = device->graphics_queue;
	}
	else
	{
		vkGetDeviceQueue(device->logical_device, device->present_family_index,
							0, &device->present_queue);

		if (device->present_queue == VK_NULL_HANDLE)
		{
			fprintf(stderr, "Error: Unable to retrieve present queue!\n");
			return -1;
		}
	}

	return 0;
}
