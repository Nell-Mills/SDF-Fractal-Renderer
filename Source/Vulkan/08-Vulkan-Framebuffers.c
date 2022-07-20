#include "08-Vulkan-Framebuffers.h"

// Create Vulkan framebuffers:
int initialize_vulkan_framebuffers(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanPipeline *pipeline,
	FracRenderVulkanFramebuffers *framebuffers)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Initializing Vulkan framebuffers and G-buffer...\n");

	// Create the swapchain framebuffers:
	printf(" ---> Creating swapchain framebuffers.\n");
	if (create_swapchain_framebuffers(device, swapchain, pipeline, framebuffers) != 0)
	{
		return -1;
	}

	// Create G-buffer image and image views:
	printf(" ---> Creating G-buffer images and image views.\n");
	if (create_g_buffer_images(device, swapchain, framebuffers) != 0)
	{
		return -1;
	}

	// Create G-buffer:
	printf(" ---> Creating G-buffer.\n");
	if (create_g_buffer(device, swapchain, pipeline, framebuffers) != 0)
	{
		return -1;
	}

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	return 0;
}

// Destroy Vulkan framebuffer structure:
void destroy_vulkan_framebuffers(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanFramebuffers *framebuffers)
{
	printf(" ---> Destroying Vulkan framebuffers and G-buffer.\n");

	// Destroy framebuffers and free memory:
	if (framebuffers->framebuffers)
	{
		for (uint32_t i = 0; i < swapchain->num_swapchain_images; i++)
		{
			if (framebuffers->framebuffers[i] != VK_NULL_HANDLE)
			{
				vkDestroyFramebuffer(device->logical_device,
					framebuffers->framebuffers[i], NULL);
			}
		}
		free(framebuffers->framebuffers);
	}

	// Destroy G-buffer:
	if (framebuffers->g_buffer != VK_NULL_HANDLE)
	{
		vkDestroyFramebuffer(device->logical_device, framebuffers->g_buffer, NULL);
	}

	// Destroy G-buffer views and free memory:
	if (framebuffers->g_buffer_image_views)
	{
		for (uint32_t i = 0; i < framebuffers->num_g_buffer_images; i++)
		{
			if (framebuffers->g_buffer_image_views[i] != VK_NULL_HANDLE)
			{
				vkDestroyImageView(device->logical_device,
					framebuffers->g_buffer_image_views[i], NULL);
			}
		}
		free(framebuffers->g_buffer_image_views);
	}

	// Destroy G-buffer images and free memory:
	if (framebuffers->g_buffer_images)
	{
		for (uint32_t i = 0; i < framebuffers->num_g_buffer_images; i++)
		{
			if (framebuffers->g_buffer_images[i] != VK_NULL_HANDLE)
			{
				vkDestroyImage(device->logical_device,
					framebuffers->g_buffer_images[i], NULL);
				vkFreeMemory(device->logical_device,
					framebuffers->g_buffer_image_memory[i], NULL);
			}
		}
		free(framebuffers->g_buffer_images);
		free(framebuffers->g_buffer_image_memory);
	}

	// Free memory for G-buffer formats:
	if (framebuffers->g_buffer_formats)
	{
		free(framebuffers->g_buffer_formats);
	}
}

// Create framebuffers for the swapchain:
int create_swapchain_framebuffers(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanPipeline *pipeline,
	FracRenderVulkanFramebuffers *framebuffers)
{
	// Allocate memory for framebuffers. Matching free is in destroy_vulkan_framebuffers:
	framebuffers->framebuffers = malloc(swapchain->num_swapchain_images *
							sizeof(VkFramebuffer));

	// Loop through the swapchain views:
	for (uint32_t i = 0; i < swapchain->num_swapchain_images; i++)
	{
		// Create attachments:
		VkImageView attachments[1] = {
			swapchain->swapchain_image_views[i]
		};

		// Define framebuffer creation info:
		VkFramebufferCreateInfo framebuffer_info;
		framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebuffer_info.pNext			= NULL;
		framebuffer_info.flags			= 0;
		framebuffer_info.renderPass		= pipeline->colour_render_pass;
		framebuffer_info.attachmentCount	= 1;
		framebuffer_info.pAttachments		= attachments;
		framebuffer_info.width			= swapchain->swapchain_extent.width;
		framebuffer_info.height			= swapchain->swapchain_extent.height;
		framebuffer_info.layers			= 1;

		// Create framebuffer:
		if (vkCreateFramebuffer(device->logical_device, &framebuffer_info,
			NULL, &framebuffers->framebuffers[i]) != VK_SUCCESS)
		{
			fprintf(stderr, "Error: Unable to create framebuffer %d!\n", i);
			return -1;
		}
	}

	return 0;
}

// Create G-buffer images and image views:
int create_g_buffer_images(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
						FracRenderVulkanFramebuffers *framebuffers)
{
	// Allocate memory for images and image views (free in destroy_vulkan_framebuffers):
	framebuffers->g_buffer_images = malloc(framebuffers->num_g_buffer_images *
								sizeof(VkImage));
	framebuffers->g_buffer_image_memory = malloc(framebuffers->num_g_buffer_images *
								sizeof(VkDeviceMemory));
	framebuffers->g_buffer_image_views = malloc(framebuffers->num_g_buffer_images *
								sizeof(VkImageView));

	// Initialize all handles to VK_NULL_HANDLE:
	for (uint32_t i = 0; i < framebuffers->num_g_buffer_images; i++)
	{
		framebuffers->g_buffer_images[i] = VK_NULL_HANDLE;
		framebuffers->g_buffer_image_memory[i] = VK_NULL_HANDLE;
		framebuffers->g_buffer_image_views[i] = VK_NULL_HANDLE;
	}

	// Loop through images:
	for (uint32_t i = 0; i < framebuffers->num_g_buffer_images; i++)
	{
		// Define image creation info:
		VkImageCreateInfo image_info;
		image_info.sType			= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image_info.pNext			= NULL;
		image_info.flags			= 0;
		image_info.imageType			= VK_IMAGE_TYPE_2D;
		image_info.format			= framebuffers->g_buffer_formats[i];
		image_info.extent.width			= swapchain->swapchain_extent.width;
		image_info.extent.height		= swapchain->swapchain_extent.height;
		image_info.extent.depth			= 1;
		image_info.mipLevels			= 1;
		image_info.arrayLayers			= 1;
		image_info.samples			= VK_SAMPLE_COUNT_1_BIT;
		image_info.tiling			= VK_IMAGE_TILING_OPTIMAL;
		image_info.usage			= VK_IMAGE_USAGE_SAMPLED_BIT |
							VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		image_info.sharingMode			= VK_SHARING_MODE_EXCLUSIVE;
		image_info.queueFamilyIndexCount	= 0;
		image_info.pQueueFamilyIndices		= NULL;
		image_info.initialLayout		= VK_IMAGE_LAYOUT_UNDEFINED;

		// Create image:
		if (vkCreateImage(device->logical_device, &image_info, NULL,
			&framebuffers->g_buffer_images[i]) != VK_SUCCESS)
		{
			fprintf(stderr, "Error: Unable to create G-buffer image %d!\n", i);
			return -1;
		}

		// Get memory requirements of image:
		VkMemoryRequirements memory_requirements;
		vkGetImageMemoryRequirements(device->logical_device,
			framebuffers->g_buffer_images[i], &memory_requirements);

		// Allocate memory for image:
		VkMemoryAllocateInfo allocate_info;
		allocate_info.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocate_info.pNext		= NULL;
		allocate_info.allocationSize	= memory_requirements.size;

		// Find suitable memory type for image:
		VkPhysicalDeviceMemoryProperties memory_properties;
		vkGetPhysicalDeviceMemoryProperties(device->physical_device, &memory_properties);

		VkMemoryPropertyFlags required_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		int success_flag = -1;
		for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++)
		{
			if ((memory_requirements.memoryTypeBits & (1 << i)) &&
				((memory_properties.memoryTypes[i].propertyFlags &
				required_properties) == required_properties))
			{
				allocate_info.memoryTypeIndex = i;
				success_flag = 0;
				break;
			}
		}
		if (success_flag != 0)
		{
			fprintf(stderr, "Error: No suitable memory type found for G-buffer "
									"image %d!\n", i);
			return -1;
		}

		// Allocate memory for image:
		if (vkAllocateMemory(device->logical_device, &allocate_info, NULL,
			&framebuffers->g_buffer_image_memory[i]) != VK_SUCCESS)
		{
			fprintf(stderr, "Error: Unable to allocate memory for G-buffer "
									"image %d!\n", i);
			return -1;
		}

		// Bind image memory:
		vkBindImageMemory(device->logical_device, framebuffers->g_buffer_images[i],
						framebuffers->g_buffer_image_memory[i], 0);

		// Define image view creation info:
		VkImageViewCreateInfo view_info;
		view_info.sType			= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view_info.pNext			= NULL;
		view_info.flags			= 0;
		view_info.image			= framebuffers->g_buffer_images[i];
		view_info.viewType		= VK_IMAGE_VIEW_TYPE_2D;
		view_info.format		= framebuffers->g_buffer_formats[i];

		view_info.components.r	= VK_COMPONENT_SWIZZLE_IDENTITY;
		view_info.components.g	= VK_COMPONENT_SWIZZLE_IDENTITY;
		view_info.components.b	= VK_COMPONENT_SWIZZLE_IDENTITY;
		view_info.components.a	= VK_COMPONENT_SWIZZLE_IDENTITY;

		view_info.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
		view_info.subresourceRange.baseMipLevel		= 0;
		view_info.subresourceRange.levelCount		= 1;
		view_info.subresourceRange.baseArrayLayer	= 0;
		view_info.subresourceRange.layerCount		= 1;

		// Create image view:
		if (vkCreateImageView(device->logical_device, &view_info, NULL,
				&framebuffers->g_buffer_image_views[i]) != VK_SUCCESS)
		{
			fprintf(stderr, "Error: Unable to create image view for G-buffer "
									"image %d!\n", i);
			return -1;
		}
	}

	return 0;
}

// Create G-buffer:
int create_g_buffer(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
	FracRenderVulkanPipeline *pipeline, FracRenderVulkanFramebuffers *framebuffers)
{
	// Create attachments:
	VkImageView attachments[1] = {
		framebuffers->g_buffer_image_views[0]
	};

	// Define G-buffer creation info:
	VkFramebufferCreateInfo g_buffer_info;
	g_buffer_info.sType		= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	g_buffer_info.pNext		= NULL;
	g_buffer_info.flags		= 0;
	g_buffer_info.renderPass	= pipeline->geometry_render_pass;
	g_buffer_info.attachmentCount	= 1;
	g_buffer_info.pAttachments	= attachments;
	g_buffer_info.width		= swapchain->swapchain_extent.width;
	g_buffer_info.height		= swapchain->swapchain_extent.height;
	g_buffer_info.layers		= 1;

	// Create G-buffer:
	if (vkCreateFramebuffer(device->logical_device, &g_buffer_info, NULL,
					&framebuffers->g_buffer) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to create G-buffer!\n");
		return -1;
	}

	return 0;
}
