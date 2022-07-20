#include "06-Vulkan-Descriptors.h"

// Create Vulkan descriptors:
int initialize_vulkan_descriptors(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Initializing Vulkan descriptors...\n");

	// Create descriptor pool:
	printf(" ---> Creating descriptor pool.\n");
	if (create_descriptor_pool(device, descriptors) != 0)
	{
		fprintf(stderr, "Error: Unable to create descriptor pool!\n");
		return -1;
	}

	// Create scene buffer:
	printf(" ---> Creating scene buffer.\n");
	if (create_scene_buffer(device, descriptors) != 0)
	{
		fprintf(stderr, "Error: Unable to create scene buffer!\n");
		return -1;
	}

	// Create scene descriptor:
	printf(" ---> Creating scene descriptor.\n");
	if (create_scene_descriptor(device, descriptors) != 0)
	{
		fprintf(stderr, "Error: Unable to create scene descriptor!\n");
		return -1;
	}

	// Create G-buffer descriptors:
	printf(" ---> Creating G-buffer descriptors.\n");
	if (create_g_buffer_descriptors(device, descriptors) != 0)
	{
		fprintf(stderr, "Error: Unable to create G-buffer descriptors!\n");
		return -1;
	}

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

	// Destroy scene descriptor layout:
	if (descriptors->scene_descriptor_layout != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorSetLayout(device->logical_device,
				descriptors->scene_descriptor_layout, NULL);
	}

	// Destroy scene uniform buffer:
	if (descriptors->scene_buffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(device->logical_device, descriptors->scene_buffer, NULL);
	}
	if (descriptors->scene_memory != VK_NULL_HANDLE)
	{
		vkFreeMemory(device->logical_device, descriptors->scene_memory, NULL);
	}

	// Destroy G-buffer descriptor layouts and free memory:
	if (descriptors->g_buffer_descriptor_layouts)
	{
		for (uint32_t i = 0; i < descriptors->num_g_buffer_descriptors; i++)
		{
			if (descriptors->g_buffer_descriptor_layouts[i] != VK_NULL_HANDLE)
			{
				vkDestroyDescriptorSetLayout(device->logical_device,
					descriptors->g_buffer_descriptor_layouts[i], NULL);
			}
		}
		free(descriptors->g_buffer_descriptor_layouts);
	}
	if (descriptors->g_buffer_descriptors)
	{
		free(descriptors->g_buffer_descriptors);
	}

	// Destroy descriptor pool:
	if (descriptors->descriptor_pool != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorPool(device->logical_device,
			descriptors->descriptor_pool, NULL);
	}
}

// Create descriptor pool:
int create_descriptor_pool(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors)
{
	// Define the descriptor pool types:
	VkDescriptorPoolSize pools[2];
	pools[0].type			= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	pools[0].descriptorCount	= 2048;
	pools[1].type			= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	pools[1].descriptorCount	= 2048;

	// Define the descriptor pool creation info:
	VkDescriptorPoolCreateInfo pool_info;
	pool_info.sType		= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.pNext		= NULL;
	pool_info.flags		= 0;
	pool_info.maxSets	= 1024;
	pool_info.poolSizeCount	= 2;
	pool_info.pPoolSizes	= pools;

	// Create the descriptor pool:
	if (vkCreateDescriptorPool(device->logical_device, &pool_info, NULL,
				&descriptors->descriptor_pool) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to create descriptor pool!\n");
		return -1;
	}

	return 0;
}

// Create scene buffer:
int create_scene_buffer(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors)
{
	// Define buffer creation info:
	VkBufferCreateInfo buffer_info;
	buffer_info.sType			= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.pNext			= NULL;
	buffer_info.flags			= 0;
	buffer_info.size			= sizeof(FracRenderVulkanSceneUniform);
	buffer_info.usage			= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT |
						VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	buffer_info.sharingMode			= VK_SHARING_MODE_EXCLUSIVE;
	buffer_info.queueFamilyIndexCount	= 0;
	buffer_info.pQueueFamilyIndices		= NULL;

	// Create buffer:
	if (vkCreateBuffer(device->logical_device, &buffer_info, NULL,
			&descriptors->scene_buffer) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to create scene buffer!\n");
		return -1;
	}

	// Get buffer memory requirements:
	VkMemoryRequirements memory_requirements;
	vkGetBufferMemoryRequirements(device->logical_device,
		descriptors->scene_buffer, &memory_requirements);

	// Get memory allocation info:
	VkMemoryAllocateInfo allocate_info;
	allocate_info.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocate_info.pNext		= NULL;
	allocate_info.allocationSize	= memory_requirements.size;

	// Find suitable memory type for buffer:
	VkPhysicalDeviceMemoryProperties memory_properties;
	vkGetPhysicalDeviceMemoryProperties(device->physical_device, &memory_properties);

	VkMemoryPropertyFlags required_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
						VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
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
		fprintf(stderr, "Error: No suitable memory type found for scene buffer!\n");
		return -1;
	}

	// Allocate memory for the buffer:
	if (vkAllocateMemory(device->logical_device, &allocate_info, NULL,
				&descriptors->scene_memory) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to allocate memory for scene buffer!\n");
		return -1;
	}

	// Bind the buffer memory:
	vkBindBufferMemory(device->logical_device, descriptors->scene_buffer,
						descriptors->scene_memory, 0);

	return 0;
}

// Create scene descriptor:
int create_scene_descriptor(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors)
{
	// Create array of descriptor set layout bindings:
	VkDescriptorSetLayoutBinding bindings[1];
	bindings[0].binding		= 0;
	bindings[0].descriptorType	= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	bindings[0].descriptorCount	= 1;
	bindings[0].stageFlags		= VK_SHADER_STAGE_VERTEX_BIT |
					VK_SHADER_STAGE_FRAGMENT_BIT;
	bindings[0].pImmutableSamplers	= NULL;

	// Create descriptor set layout:
	VkDescriptorSetLayoutCreateInfo layout_info;
	layout_info.sType		= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.pNext		= NULL;
	layout_info.flags		= 0;
	layout_info.bindingCount	= 1;
	layout_info.pBindings		= bindings;

	if (vkCreateDescriptorSetLayout(device->logical_device, &layout_info,
		NULL, &descriptors->scene_descriptor_layout) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to create scene descriptor set layout!\n");
		return -1;
	}

	// Allocate descriptor set:
	VkDescriptorSetAllocateInfo allocate_info;
	allocate_info.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocate_info.pNext			= NULL;
	allocate_info.descriptorPool		= descriptors->descriptor_pool;
	allocate_info.descriptorSetCount	= 1;
	allocate_info.pSetLayouts		= &descriptors->scene_descriptor_layout;

	if (vkAllocateDescriptorSets(device->logical_device, &allocate_info,
				&descriptors->scene_descriptor) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to allocate scene descriptor set!\n");
		return -1;
	}

	// Create descriptor set:
	VkDescriptorBufferInfo scene_UBO_info;
	scene_UBO_info.buffer	= descriptors->scene_buffer;
	scene_UBO_info.offset	= 0;
	scene_UBO_info.range	= VK_WHOLE_SIZE;

	VkWriteDescriptorSet descriptor_write[1];
	descriptor_write[0].sType		= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptor_write[0].pNext		= NULL;
	descriptor_write[0].dstSet		= descriptors->scene_descriptor;
	descriptor_write[0].dstBinding		= 0;
	descriptor_write[0].dstArrayElement	= 0;
	descriptor_write[0].descriptorCount	= 1;
	descriptor_write[0].descriptorType	= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptor_write[0].pImageInfo		= NULL;
	descriptor_write[0].pBufferInfo		= &scene_UBO_info;
	descriptor_write[0].pTexelBufferView	= NULL;

	// Update the descriptor sets:
	vkUpdateDescriptorSets(device->logical_device, 1, descriptor_write, 0, NULL);

	return 0;
}

// Create G-buffer descriptors:
int create_g_buffer_descriptors(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors)
{
	return 0;
}
