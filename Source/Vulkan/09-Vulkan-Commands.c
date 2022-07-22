#include "09-Vulkan-Commands.h"

// Create Vulkan command structure:
int initialize_vulkan_commands(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanCommands *commands)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Initializing Vulkan command pool, fences and semaphores...\n");

	// Create command pool:
	printf(" ---> Creating command pool.\n");
	if (create_command_pool(device, commands) != 0)
	{
		return -1;
	}

	// Create command buffers:
	printf(" ---> Creating command buffers.\n");
	if (create_command_buffers(device, swapchain, commands) != 0)
	{
		return -1;
	}

	// Create fences:
	printf(" ---> Creating fences.\n");
	if (create_fences(device, swapchain, commands) != 0)
	{
		return -1;
	}

	// Create semaphores:
	printf(" ---> Creating semaphores.\n");
	if (create_semaphores(device, commands) != 0)
	{
		return -1;
	}

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	return 0;
}

// Destroy Vulkan command structure:
void destroy_vulkan_commands(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
							FracRenderVulkanCommands *commands)
{
	printf(" ---> Destroying Vulkan command pool, fences and semaphores.\n");

	// Destroy command pool:
	if (commands->command_pool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(device->logical_device, commands->command_pool, NULL);
	}

	// Free memory for command buffers:
	if (commands->command_buffers)
	{
		free(commands->command_buffers);
	}

	// Destroy fences and free memory:
	if (commands->fences)
	{
		for (uint32_t i = 0; i < swapchain->num_swapchain_images; i++)
		{
			if (commands->fences[i] != VK_NULL_HANDLE)
			{
				vkDestroyFence(device->logical_device, commands->fences[i], NULL);
			}
		}
		free(commands->fences);
	}

	// Destroy semaphores:
	if (commands->image_available != VK_NULL_HANDLE)
	{
		vkDestroySemaphore(device->logical_device, commands->image_available, NULL);
	}
	if (commands->render_finished != VK_NULL_HANDLE)
	{
		vkDestroySemaphore(device->logical_device, commands->render_finished, NULL);
	}
}

// Create command pool:
int create_command_pool(FracRenderVulkanDevice *device, FracRenderVulkanCommands *commands)
{
	// Define command pool creation info:
	VkCommandPoolCreateInfo pool_info;
	memset(&pool_info, 0, sizeof(VkCommandPoolCreateInfo));
	pool_info.sType			= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.pNext			= NULL;
	pool_info.flags			= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT |
					VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	pool_info.queueFamilyIndex	= device->graphics_family_index;

	// Create command pool:
	if (vkCreateCommandPool(device->logical_device, &pool_info, NULL,
				&commands->command_pool) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to create command pool!\n");
		return -1;
	}

	return 0;
}

// Create command buffers:
int create_command_buffers(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
							FracRenderVulkanCommands *commands)
{
	// Allocate memory for command buffers (free in destroy_vulkan_commands):
	commands->command_buffers = malloc(swapchain->num_swapchain_images *
						sizeof(VkCommandBuffer));

	// Loop through swapchain images:
	for (uint32_t i = 0; i < swapchain->num_swapchain_images; i++)
	{
		// Define command buffer allocation info:
		VkCommandBufferAllocateInfo allocate_info;
		memset(&allocate_info, 0, sizeof(VkCommandBufferAllocateInfo));
		allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocate_info.pNext			= NULL;
		allocate_info.commandPool		= commands->command_pool;
		allocate_info.level			= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocate_info.commandBufferCount	= 1;

		// Allocate command buffer from pool:
		if (vkAllocateCommandBuffers(device->logical_device, &allocate_info,
					&commands->command_buffers[i]) != VK_SUCCESS)
		{
			fprintf(stderr, "Error: Unable to allocate command buffer %d!\n", i);
			return -1;
		}
	}

	return 0;
}

// Create fences:
int create_fences(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
						FracRenderVulkanCommands *commands)
{
	// Allocate memory for fences (free in destroy_vulkan_commands):
	commands->fences = malloc(swapchain->num_swapchain_images * sizeof(VkFence));

	// Initialize the fences to VK_NULL_HANDLE:
	for (uint32_t i = 0; i < swapchain->num_swapchain_images; i++)
	{
		commands->fences[i] = VK_NULL_HANDLE;
	}

	// Loop through swapchain images:
	for (uint32_t i = 0; i < swapchain->num_swapchain_images; i++)
	{
		// Define fence creation info:
		VkFenceCreateInfo fence_info;
		memset(&fence_info, 0, sizeof(VkFenceCreateInfo));
		fence_info.sType	= VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fence_info.pNext	= NULL;
		fence_info.flags	= VK_FENCE_CREATE_SIGNALED_BIT;

		// Create fence:
		if (vkCreateFence(device->logical_device, &fence_info, NULL,
					&commands->fences[i]) != VK_SUCCESS)
		{
			fprintf(stderr, "Error: Unable to create fence %d!\n", i);
			return -1;
		}
	}

	return 0;
}

// Create semaphores:
int create_semaphores(FracRenderVulkanDevice *device, FracRenderVulkanCommands *commands)
{
	// Create semaphore to signal that an image is available:
	VkSemaphoreCreateInfo image_available_info;
	memset(&image_available_info, 0, sizeof(VkSemaphoreCreateInfo));
	image_available_info.sType	= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	image_available_info.pNext	= NULL;
	image_available_info.flags	= 0;

	if (vkCreateSemaphore(device->logical_device, &image_available_info,
			NULL, &commands->image_available) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to create \"image available\" semaphore!\n");
		return -1;
	}

	// Create semaphore to signal that rendering is finished:
	VkSemaphoreCreateInfo render_finished_info;
	memset(&render_finished_info, 0, sizeof(VkSemaphoreCreateInfo));
	render_finished_info.sType	= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	render_finished_info.pNext	= NULL;
	render_finished_info.flags	= 0;

	if (vkCreateSemaphore(device->logical_device, &render_finished_info,
			NULL, &commands->render_finished) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to create \"render finished\" semaphore!\n");
		return -1;
	}

	return 0;
}
