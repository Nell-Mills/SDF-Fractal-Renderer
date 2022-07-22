#include "05-Vulkan-Swapchain.h"

// Create Vulkan swapchain and associated images:
int initialize_vulkan_swapchain(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
							FracRenderVulkanSwapchain *swapchain)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Initializing Vulkan swapchain...\n");

	printf(" ---> Creating swapchain.\n");
	if (create_swapchain(base, device, swapchain, VK_NULL_HANDLE) != 0)
	{
		return -1;
	}

	printf(" ---> Creating swapchain images and image views.\n");
	if (create_swapchain_images(device, swapchain) != 0)
	{
		return -1;
	}

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	return 0;
}

// Destroy Vulkan swapchain structure:
void destroy_vulkan_swapchain(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain)
{
	printf(" ---> Destroying Vulkan swapchain.\n");

	// Destroy swapchain image views and free memory:
	if (swapchain->swapchain_image_views)
	{
		for (uint32_t i = 0; i < swapchain->num_swapchain_images; i++)
		{
			vkDestroyImageView(device->logical_device,
				swapchain->swapchain_image_views[i], NULL);
		}
		free(swapchain->swapchain_image_views);
	}

	// Free memory for swapchain images if necessary:
	if (swapchain->swapchain_images)
	{
		free(swapchain->swapchain_images);
	}

	// Destroy swapchain:
	if (swapchain->swapchain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(device->logical_device, swapchain->swapchain, NULL);
	}
}

// Recreate Vulkan swapchain and associated images:
int recreate_vulkan_swapchain(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
							FracRenderVulkanSwapchain *swapchain)
{
	// Save current format and extent to see what's changed:
	VkFormat old_format = swapchain->swapchain_format;
	VkExtent2D old_extent = swapchain->swapchain_extent;

	// Keep old swapchain to use in creation of new one:
	VkSwapchainKHR old_swapchain = swapchain->swapchain;

	// Destroy the image views and discard the images:
	for (uint32_t i = 0; i < swapchain->num_swapchain_images; i++)
	{
		vkDestroyImageView(device->logical_device,
			swapchain->swapchain_image_views[i], NULL);
		swapchain->swapchain_image_views[i] = VK_NULL_HANDLE;
		swapchain->swapchain_images[i] = VK_NULL_HANDLE;
	}
	free(swapchain->swapchain_image_views);
	free(swapchain->swapchain_images);

	// Create new swapchain:
	if (create_swapchain(base, device, swapchain, old_swapchain) != 0)
	{
		// Restore old swapchain so it gets destroyed properly:
		swapchain->swapchain = old_swapchain;

		fprintf(stderr, "Error: Unable to recreate swapchain!\n");
		return -1;
	}

	// Destroy old swapchain:
	vkDestroySwapchainKHR(device->logical_device, old_swapchain, NULL);

	// Get new swapchain images and image views:
	if (create_swapchain_images(device, swapchain) != 0)
	{
		fprintf(stderr, "Error: Unable to create new swapchain images!\n");
		return -1;
	}

	int return_value = 0;

	// Return a value depending on which things have changed:
	if ((old_extent.width != swapchain->swapchain_extent.width) ||
		(old_extent.height != swapchain->swapchain_extent.height))
	{
		return_value = 1;
	}
	if (old_format != swapchain->swapchain_format)
	{
		if (return_value == 1)
		{
			return_value = 3;
		}
		else
		{
			return_value = 2;
		}
	}

	return return_value;
}

// Create Vulkan swapchain:
int create_swapchain(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, VkSwapchainKHR old_swapchain)
{
	// Get surface formats:
	uint32_t num_formats = 0;
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(device->physical_device,
			base->surface, &num_formats, NULL) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to enumerate surface formats!\n");
		return -1;
	}

	VkSurfaceFormatKHR *formats = malloc(num_formats * sizeof(VkSurfaceFormatKHR));
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(device->physical_device,
			base->surface, &num_formats, formats) != VK_SUCCESS)
	{
		// Free memory:
		free(formats);

		fprintf(stderr, "Error: Unable to get surface formats!\n");
		return -1;
	}

	// Choose a format:
	if (num_formats < 1)
	{
		// Free memory:
		free(formats);

		fprintf(stderr, "Error: No surface formats to choose from!\n");
		return -1;
	}

	VkSurfaceFormatKHR chosen_format = formats[0];
	for (uint32_t i = 0; i < num_formats; i++)
	{
		if ((formats[i].format == VK_FORMAT_R8G8B8A8_SRGB) &&
			(formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR))
		{
			// Ideal, go with this:
			chosen_format = formats[i];
			break;
		}

		if ((formats[i].format == VK_FORMAT_B8G8R8A8_SRGB) &&
			(formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR))
		{
			// Also good:
			chosen_format = formats[i];
			break;
		}
	}

	swapchain->swapchain_format = chosen_format.format;

	// Free memory:
	free(formats);

	// Get present modes:
	uint32_t num_modes = 0;
	if (vkGetPhysicalDeviceSurfacePresentModesKHR(device->physical_device,
				base->surface, &num_modes, NULL) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to enumerate surface present modes!\n");
		return -1;
	}

	VkPresentModeKHR *modes = malloc(num_modes * sizeof(VkPresentModeKHR));
	if (vkGetPhysicalDeviceSurfacePresentModesKHR(device->physical_device,
				base->surface, &num_modes, modes) != VK_SUCCESS)
	{
		// Free memory:
		free(modes);

		fprintf(stderr, "Error: Unable to get surface present modes!\n");
		return -1;
	}

	// Choose a present mode:
	if (num_modes < 1)
	{
		// Free memory:
		free(modes);

		fprintf(stderr, "Error: No present modes to choose from!\n");
		return -1;
	}

	VkPresentModeKHR chosen_mode = VK_PRESENT_MODE_FIFO_KHR; // Guaranteed to be available.
	for (uint32_t i = 0; i < num_modes; i++)
	{
		if (modes[i] == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
		{
			chosen_mode = modes[i];
			break;
		}
	}

	// Free memory:
	free(modes);

	// Get image count range for the device:
	VkSurfaceCapabilitiesKHR capabilities;
	if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->physical_device,
				base->surface, &capabilities) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to get physical device surface capabilities!\n");
	}

	// Choose an image count within the allowed range:
	uint32_t num_images = 2; // Double buffering.

	if (num_images < (capabilities.minImageCount + 1))
	{
		num_images = capabilities.minImageCount + 1;
	}
	if ((num_images > capabilities.maxImageCount) && (capabilities.maxImageCount > 0))
	{
		num_images = capabilities.maxImageCount;
	}

	// Find the swapchain extent:
	swapchain->swapchain_extent = capabilities.currentExtent;
	if (swapchain->swapchain_extent.width == UINT32_MAX)
	{
		// Driver does not have current extent information:
		int width, height;
		glfwGetFramebufferSize(base->window, &width, &height);

		// Make sure extent is within supported limits for the device:
		if (swapchain->swapchain_extent.width < capabilities.minImageExtent.width)
		{
			swapchain->swapchain_extent.width = capabilities.minImageExtent.width;
		}
		if (swapchain->swapchain_extent.width > capabilities.maxImageExtent.width)
		{
			swapchain->swapchain_extent.width = capabilities.maxImageExtent.width;
		}
		if (swapchain->swapchain_extent.height < capabilities.minImageExtent.height)
		{
			swapchain->swapchain_extent.height = capabilities.minImageExtent.height;
		}
		if (swapchain->swapchain_extent.height > capabilities.maxImageExtent.height)
		{
			swapchain->swapchain_extent.height = capabilities.maxImageExtent.height;
		}
	}

	// Define the swapchain creation information:
	VkSwapchainCreateInfoKHR swapchain_info;
	memset(&swapchain_info, 0, sizeof(VkSwapchainCreateInfoKHR));
	swapchain_info.sType			= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_info.pNext			= NULL;
	swapchain_info.flags			= 0;
	swapchain_info.surface			= base->surface;
	swapchain_info.minImageCount		= num_images;
	swapchain_info.imageFormat		= chosen_format.format;
	swapchain_info.imageColorSpace		= chosen_format.colorSpace;
	swapchain_info.imageExtent		= swapchain->swapchain_extent;
	swapchain_info.imageArrayLayers		= 1;
	swapchain_info.imageUsage		= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchain_info.preTransform		= capabilities.currentTransform;
	swapchain_info.compositeAlpha		= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_info.presentMode		= chosen_mode;
	swapchain_info.clipped			= VK_TRUE;
	swapchain_info.oldSwapchain		= old_swapchain;

	// Use exclusive image sharing if only one queue, otherwise concurrent:
	const uint32_t queue_family_indices[] = {
		device->graphics_family_index,
		device->present_family_index
	};
	if (device->graphics_family_index == device->present_family_index)
	{
		swapchain_info.imageSharingMode		= VK_SHARING_MODE_EXCLUSIVE;
		swapchain_info.queueFamilyIndexCount	= 0;
		swapchain_info.pQueueFamilyIndices	= NULL;
	}
	else
	{
		swapchain_info.imageSharingMode		= VK_SHARING_MODE_CONCURRENT;
		swapchain_info.queueFamilyIndexCount	= 2;
		swapchain_info.pQueueFamilyIndices	= queue_family_indices;
	}

	// Create the swapchain:
	if (vkCreateSwapchainKHR(device->logical_device, &swapchain_info, NULL,
					&swapchain->swapchain) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to create swapchain!\n");
		return -1;
	}

	return 0;
}

// Create swapchain images and image views:
int create_swapchain_images(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain)
{
	// Get number of swapchain images:
	if (vkGetSwapchainImagesKHR(device->logical_device, swapchain->swapchain,
			&swapchain->num_swapchain_images, NULL) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to get number of swapchain images!\n");
		return -1;
	}

	// Get swapchain images. Free for this malloc is in destroy_vulkan_swapchain:
	swapchain->swapchain_images = malloc(swapchain->num_swapchain_images * sizeof(VkImage));
	if (vkGetSwapchainImagesKHR(device->logical_device, swapchain->swapchain,
		&swapchain->num_swapchain_images, swapchain->swapchain_images) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to get number of swapchain images!\n");
		return -1;
	}

	// For each swapchain image, create an image view. Free is in destroy_vulkan_swapchain:
	swapchain->swapchain_image_views = malloc(swapchain->num_swapchain_images *
								sizeof(VkImageView));
	for (uint32_t i = 0; i < swapchain->num_swapchain_images; i++)
	{
		swapchain->swapchain_image_views[i] = VK_NULL_HANDLE;
	}
	for (uint32_t i = 0; i < swapchain->num_swapchain_images; i++)
	{
		// Define image view creation info:
		VkImageViewCreateInfo view_info;
		memset(&view_info, 0, sizeof(VkImageViewCreateInfo));
		view_info.sType			= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view_info.pNext			= NULL;
		view_info.flags			= 0;
		view_info.image			= swapchain->swapchain_images[i];
		view_info.viewType		= VK_IMAGE_VIEW_TYPE_2D;
		view_info.format		= swapchain->swapchain_format;

		view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		view_info.subresourceRange.baseMipLevel = 0;
		view_info.subresourceRange.levelCount = 1;
		view_info.subresourceRange.baseArrayLayer = 0;
		view_info.subresourceRange.layerCount = 1;

		// Create the image view:
		if (vkCreateImageView(device->logical_device, &view_info, NULL,
			&swapchain->swapchain_image_views[i]) != VK_SUCCESS)
		{
			fprintf(stderr, "Error: Unable to create image views for swapchain!\n");
			return -1;
		}
	}

	return 0;
}
