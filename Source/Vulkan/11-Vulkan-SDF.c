#include "11-Vulkan-SDF.h"

// Initialize SDF descriptor layout, descriptor and device memory:
int initialize_vulkan_sdf(FracRenderSDF *sdf, FracRenderVulkanSDF *sdf_vulkan,
	FracRenderVulkanDevice *device, FracRenderVulkanDescriptors *descriptors)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Initializing SDF Vulkan descriptors...\n");

	// Create SDF buffer:
	printf(" ---> Creating SDF data buffer.\n");
	if (create_sdf_buffer(sdf, sdf_vulkan, device) != 0)
	{
		return -1;
	}

	// Create SDF descriptor layout:
	printf(" ---> Creating SDF descriptor set layout.\n");
	if (create_sdf_descriptor_layout(sdf_vulkan, device) != 0)
	{
		return -1;
	}

	// Create SDF descriptor:
	printf(" ---> Creating SDF descriptor set.\n");
	if (create_sdf_descriptor(sdf_vulkan, device, descriptors) != 0)
	{
		return -1;
	}

	printf("... done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	return 0;
}

// Destroy SDF Vulkan structure:
void destroy_vulkan_sdf(FracRenderVulkanDevice *device, FracRenderVulkanSDF *sdf_vulkan)
{
	// Destroy SDF descriptor layout:
	if (sdf_vulkan->descriptor_layout != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorSetLayout(device->logical_device,
			sdf_vulkan->descriptor_layout, NULL);
	}

	// Destroy SDF buffer and associated device memory:
	if (sdf_vulkan->buffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(device->logical_device, sdf_vulkan->buffer, NULL);
	}
	if (sdf_vulkan->memory != VK_NULL_HANDLE)
	{
		vkFreeMemory(device->logical_device, sdf_vulkan->memory, NULL);
	}
}

// Create buffer for SDF:
int create_sdf_buffer(FracRenderSDF *sdf, FracRenderVulkanSDF *sdf_vulkan,
					FracRenderVulkanDevice *device)
{
	// Define buffer creation info:
	size_t sdf_memory = sdf->num_voxels * sizeof(FracRenderVoxel);
	VkBufferCreateInfo buffer_info;
	memset(&buffer_info, 0, sizeof(VkBufferCreateInfo));
	buffer_info.sType			= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.pNext			= NULL;
	buffer_info.flags			= 0;
	buffer_info.size			= sdf_memory;
	buffer_info.usage			= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
						VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	buffer_info.sharingMode			= VK_SHARING_MODE_EXCLUSIVE;
	buffer_info.queueFamilyIndexCount	= 0;
	buffer_info.pQueueFamilyIndices		= NULL;

	// Create buffer:
	if (vkCreateBuffer(device->logical_device, &buffer_info, NULL,
				&sdf_vulkan->buffer) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to create SDF buffer!\n");
		return -1;
	}

	// Get buffer memory requirements:
	VkMemoryRequirements memory_requirements;
	vkGetBufferMemoryRequirements(device->logical_device,
		sdf_vulkan->buffer, &memory_requirements);

	// Get memory allocation info:
	VkMemoryAllocateInfo allocate_info;
	memset(&allocate_info, 0, sizeof(VkMemoryAllocateInfo));
	allocate_info.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocate_info.pNext		= NULL;
	allocate_info.allocationSize	= memory_requirements.size;

	printf("      - Memory needed: %lu bytes.\n", memory_requirements.size);

	// Find suitable memory type for buffer:
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
		fprintf(stderr, "Error: No suitable memory type found for SDF buffer!\n");
		return -1;
	}

	// Allocate memory for buffer:
	if (vkAllocateMemory(device->logical_device, &allocate_info, NULL,
					&sdf_vulkan->memory) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to allocate memory for SDF buffer!\n");
		return -1;
	}

	// Bind buffer memory:
	vkBindBufferMemory(device->logical_device, sdf_vulkan->buffer, sdf_vulkan->memory, 0);

	return 0;
}

// Create SDF descriptor set layout:
int create_sdf_descriptor_layout(FracRenderVulkanSDF *sdf_vulkan, FracRenderVulkanDevice *device)
{
	// Create array of descriptor set layout bindings:
	VkDescriptorSetLayoutBinding bindings[1];
	memset(bindings, 0, 1 * sizeof(VkDescriptorSetLayoutBinding));
	bindings[0].binding		= 0;
	bindings[0].descriptorType	= VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	bindings[0].descriptorCount	= 1;
	bindings[0].stageFlags		= VK_SHADER_STAGE_FRAGMENT_BIT;
	bindings[0].pImmutableSamplers	= NULL;

	// Create descriptor set layout:
	VkDescriptorSetLayoutCreateInfo layout_info;
	memset(&layout_info, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
	layout_info.sType		= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.pNext		= NULL;
	layout_info.flags		= 0;
	layout_info.bindingCount	= 1;
	layout_info.pBindings		= bindings;

	if (vkCreateDescriptorSetLayout(device->logical_device, &layout_info,
		NULL, &sdf_vulkan->descriptor_layout) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to create SDF descriptor set layout!\n");
		return -1;
	}

	return 0;
}

// Create SDF descriptor:
int create_sdf_descriptor(FracRenderVulkanSDF *sdf_vulkan, FracRenderVulkanDevice *device,
						FracRenderVulkanDescriptors *descriptors)
{
	// Allocate descriptor set:
	VkDescriptorSetAllocateInfo allocate_info;
	memset(&allocate_info, 0, sizeof(VkDescriptorSetAllocateInfo));
	allocate_info.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocate_info.pNext			= NULL;
	allocate_info.descriptorPool		= descriptors->descriptor_pool;
	allocate_info.descriptorSetCount	= 1;
	allocate_info.pSetLayouts		= &sdf_vulkan->descriptor_layout;

	if (vkAllocateDescriptorSets(device->logical_device, &allocate_info,
				&sdf_vulkan->descriptor_set) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to allocate SDF descriptor set!\n");
		return -1;
	}

	// Create descriptor set:
	VkDescriptorBufferInfo sdf_buffer_info;
	memset(&sdf_buffer_info, 0, sizeof(VkDescriptorBufferInfo));
	sdf_buffer_info.buffer	= sdf_vulkan->buffer;
	sdf_buffer_info.offset	= 0;
	sdf_buffer_info.range	= VK_WHOLE_SIZE;

	VkWriteDescriptorSet descriptor_write[1];
	memset(descriptor_write, 0, 1 * sizeof(VkWriteDescriptorSet));
	descriptor_write[0].sType		= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptor_write[0].pNext		= NULL;
	descriptor_write[0].dstSet		= sdf_vulkan->descriptor_set;
	descriptor_write[0].dstBinding		= 0;
	descriptor_write[0].dstArrayElement	= 0;
	descriptor_write[0].descriptorCount	= 1;
	descriptor_write[0].descriptorType	= VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	descriptor_write[0].pImageInfo		= NULL;
	descriptor_write[0].pBufferInfo		= &sdf_buffer_info;
	descriptor_write[0].pTexelBufferView	= NULL;

	// Update descriptor sets:
	vkUpdateDescriptorSets(device->logical_device, 1, descriptor_write, 0, NULL);

	return 0;
}

// Copy data into the SDF buffer:
int copy_sdf_data(FracRenderSDF *sdf, FracRenderVulkanSDF *sdf_vulkan,
	FracRenderVulkanDevice *device, FracRenderVulkanCommands *commands)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Copying SDF data into GPU buffer...\n");

	// Create staging buffer. First define buffer creation info:
	VkBufferCreateInfo staging_buffer_info;
	memset(&staging_buffer_info, 0, sizeof(VkBufferCreateInfo));
	staging_buffer_info.sType			= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	staging_buffer_info.pNext			= NULL;
	staging_buffer_info.flags			= 0;
	staging_buffer_info.size			= sdf->num_voxels * sizeof(FracRenderVoxel);
	staging_buffer_info.usage			= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	staging_buffer_info.sharingMode			= VK_SHARING_MODE_EXCLUSIVE;
	staging_buffer_info.queueFamilyIndexCount	= 0;
	staging_buffer_info.pQueueFamilyIndices		= NULL;

	// Create buffer:
	VkBuffer staging_buffer = VK_NULL_HANDLE;
	if (vkCreateBuffer(device->logical_device, &staging_buffer_info, NULL,
						&staging_buffer) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to create staging buffer for SDF copying!\n");
		return -1;
	}

	// Get buffer memory requirements:
	VkMemoryRequirements staging_memory_requirements;
	vkGetBufferMemoryRequirements(device->logical_device,
		staging_buffer, &staging_memory_requirements);

	// Get memory allocation info:
	VkMemoryAllocateInfo staging_allocate_info;
	memset(&staging_allocate_info, 0, sizeof(VkMemoryAllocateInfo));
	staging_allocate_info.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	staging_allocate_info.pNext		= NULL;
	staging_allocate_info.allocationSize	= staging_memory_requirements.size;

	// Find suitable memory type for buffer:
	VkPhysicalDeviceMemoryProperties staging_memory_properties;
	vkGetPhysicalDeviceMemoryProperties(device->physical_device, &staging_memory_properties);

	VkMemoryPropertyFlags required_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
						VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	int success_flag = -1;
	for (uint32_t i = 0; i < staging_memory_properties.memoryTypeCount; i++)
	{
		if ((staging_memory_requirements.memoryTypeBits & (1 << i)) &&
			((staging_memory_properties.memoryTypes[i].propertyFlags &
			required_properties) == required_properties))
		{
			staging_allocate_info.memoryTypeIndex = i;
			success_flag = 0;
			break;
		}
	}
	if (success_flag != 0)
	{
		// Destroy temporary objects:
		vkDestroyBuffer(device->logical_device, staging_buffer, NULL);

		fprintf(stderr, "Error: No suitable memory type found for staging "
							"buffer for SDF copying!\n");
		return -1;
	}

	// Allocate memory for buffer:
	VkDeviceMemory staging_memory = VK_NULL_HANDLE;
	if (vkAllocateMemory(device->logical_device, &staging_allocate_info, NULL,
						&staging_memory) != VK_SUCCESS)
	{
		// Destroy temporary objects:
		vkDestroyBuffer(device->logical_device, staging_buffer, NULL);

		fprintf(stderr, "Error: Unable to allocate memory for staging "
						"buffer for SDF copying!\n");
		return -1;
	}

	// Bind buffer memory:
	vkBindBufferMemory(device->logical_device, staging_buffer, staging_memory, 0);

	// Map staging buffer memory:
	void *staging_ptr;
	if (vkMapMemory(device->logical_device, staging_memory, 0,
		VK_WHOLE_SIZE, 0, &staging_ptr) != VK_SUCCESS)
	{
		// Destroy temporary objects:
		vkDestroyBuffer(device->logical_device, staging_buffer, NULL);
		vkFreeMemory(device->logical_device, staging_memory, NULL);

		fprintf(stderr, "Error: Unable to map memory for SDF copying!\n");
		return -1;
	}

	// Copy data into staging buffer:
	memcpy(staging_ptr, sdf->voxels, sdf->num_voxels * sizeof(FracRenderVoxel));

	// Allocate command buffer:
	VkCommandBufferAllocateInfo allocate_info;
	memset(&allocate_info, 0, sizeof(VkCommandBufferAllocateInfo));
	allocate_info.sType			= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocate_info.pNext			= NULL;
	allocate_info.commandPool		= commands->command_pool;
	allocate_info.level			= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocate_info.commandBufferCount	= 1;

	VkCommandBuffer command_buffer = VK_NULL_HANDLE;
	if (vkAllocateCommandBuffers(device->logical_device, &allocate_info,
						&command_buffer) != VK_SUCCESS)
	{
		// Destroy temporary objects:
		vkDestroyBuffer(device->logical_device, staging_buffer, NULL);
		vkFreeMemory(device->logical_device, staging_memory, NULL);

		fprintf(stderr, "Error: Unable to allocate command buffer for SDF copying!\n");
		return -1;
	}

	// Begin command recording:
	VkCommandBufferBeginInfo begin_info;
	memset(&begin_info, 0, sizeof(VkCommandBufferBeginInfo));
	begin_info.sType		= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.pNext		= NULL;
	begin_info.flags		= 0;
	begin_info.pInheritanceInfo	= NULL;

	if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS)
	{
		// Destroy temporary objects:
		vkFreeCommandBuffers(device->logical_device, commands->command_pool, 1, &command_buffer);
		vkDestroyBuffer(device->logical_device, staging_buffer, NULL);
		vkFreeMemory(device->logical_device, staging_memory, NULL);


		fprintf(stderr, "Error: Unable to begin recording commands for SDF copying!\n");
		return -1;
	}

	// Copy data from staging buffer into GPU SDF buffer:
	VkBufferCopy buffer_copy_info[1];
	memset(buffer_copy_info, 0, 1 * sizeof(VkBufferCopy));
	buffer_copy_info[0].srcOffset	= 0;
	buffer_copy_info[0].dstOffset	= 0;
	buffer_copy_info[0].size	= sdf->num_voxels * sizeof(FracRenderVoxel);

	vkCmdCopyBuffer(command_buffer, staging_buffer, sdf_vulkan->buffer, 1, buffer_copy_info);

	// Finish command recording:
	if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS)
	{
		// Destroy temporary objects:
		vkFreeCommandBuffers(device->logical_device, commands->command_pool, 1, &command_buffer);
		vkDestroyBuffer(device->logical_device, staging_buffer, NULL);
		vkFreeMemory(device->logical_device, staging_memory, NULL);

		fprintf(stderr, "Error: Unable to stop recording commands for SDF copying!\n");
		return -1;
	}

	// Create fence for submitting commands:
	VkFenceCreateInfo fence_info;
	memset(&fence_info, 0, sizeof(VkFenceCreateInfo));
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.pNext = NULL;
	fence_info.flags = 0;

	VkFence fence = VK_NULL_HANDLE;
	if (vkCreateFence(device->logical_device, &fence_info, NULL, &fence) != VK_SUCCESS)
	{
		// Destroy temporary objects:
		vkFreeCommandBuffers(device->logical_device, commands->command_pool, 1, &command_buffer);
		vkDestroyBuffer(device->logical_device, staging_buffer, NULL);
		vkFreeMemory(device->logical_device, staging_memory, NULL);

		fprintf(stderr, "Error: Unable to create fence for SDF copying!\n");
		return -1;
	}

	// Submit commands:
	VkSubmitInfo submit_info;
	memset(&submit_info, 0, sizeof(VkSubmitInfo));
	submit_info.sType			= VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.pNext			= NULL;
	submit_info.waitSemaphoreCount		= 0;
	submit_info.pWaitSemaphores		= NULL;
	submit_info.pWaitDstStageMask		= NULL;
	submit_info.commandBufferCount		= 1;
	submit_info.pCommandBuffers		= &command_buffer;
	submit_info.signalSemaphoreCount	= 0;
	submit_info.pSignalSemaphores		= NULL;

	if (vkQueueSubmit(device->graphics_queue, 1, &submit_info, fence) != VK_SUCCESS)
	{
		// Destroy temporary objects:
		vkDestroyFence(device->logical_device, fence, NULL);
		vkFreeCommandBuffers(device->logical_device, commands->command_pool, 1, &command_buffer);
		vkDestroyBuffer(device->logical_device, staging_buffer, NULL);
		vkFreeMemory(device->logical_device, staging_memory, NULL);

		fprintf(stderr, "Error: Unable to submit commands for SDF copying!\n");
		return -1;
	}

	// Wait for fence:
	if (vkWaitForFences(device->logical_device, 1, &fence, VK_TRUE, UINT64_MAX) != VK_SUCCESS)
	{
		// Destroy temporary objects:
		vkDestroyFence(device->logical_device, fence, NULL);
		vkFreeCommandBuffers(device->logical_device, commands->command_pool, 1, &command_buffer);
		vkDestroyBuffer(device->logical_device, staging_buffer, NULL);
		vkFreeMemory(device->logical_device, staging_memory, NULL);

		fprintf(stderr, "Error: Failed to wait for fence for SDF copying!\n");
		return -1;
	}

	// Destroy temporary objects:
	vkDestroyFence(device->logical_device, fence, NULL);
	vkFreeCommandBuffers(device->logical_device, commands->command_pool, 1, &command_buffer);
	vkDestroyBuffer(device->logical_device, staging_buffer, NULL);
	vkFreeMemory(device->logical_device, staging_memory, NULL);

	printf("... done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	return 0;
}
