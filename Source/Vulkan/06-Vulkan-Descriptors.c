#include "06-Vulkan-Descriptors.h"

// Create Vulkan descriptor layouts:
int initialize_vulkan_descriptor_layouts(FracRenderVulkanDevice *device,
				FracRenderVulkanDescriptors *descriptors)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Initializing Vulkan descriptor layouts and sampler...\n");

	// Create descriptor pool:
	printf(" ---> Creating descriptor pool.\n");
	if (create_descriptor_pool(device, descriptors) != 0)
	{
		return -1;
	}

	// Create sampler:
	printf(" ---> Creating sampler.\n");
	if (create_sampler(device, descriptors) != 0)
	{
		return -1;
	}

	// Create scene buffer:
	printf(" ---> Creating scene buffer.\n");
	if (create_scene_buffer(device, descriptors) != 0)
	{
		return -1;
	}

	// Create scene descriptor layout:
	printf(" ---> Creating scene descriptor layout.\n");
	if (create_scene_descriptor_layout(device, descriptors) != 0)
	{
		return -1;
	}

	// Create G-buffer descriptor layout:
	printf(" ---> Creating G-buffer descriptor layout.\n");
	if (create_g_buffer_descriptor_layout(device, descriptors) != 0)
	{
		return -1;
	}

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	return 0;
}

// Create Vulkan descriptors (after creating the framebuffers):
int initialize_vulkan_descriptors(FracRenderVulkanDevice *device,
	FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanDescriptors *descriptors)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Initializing Vulkan descriptors...\n");

	// Create scene descriptor:
	printf(" ---> Creating scene descriptor.\n");
	if (create_scene_descriptor(device, descriptors) != 0)
	{
		return -1;
	}

	// Create G-buffer descriptors:
	printf(" ---> Creating G-buffer descriptors.\n");
	if (create_g_buffer_descriptors(device, framebuffers, descriptors) != 0)
	{
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
	printf(" ---> Destroying Vulkan descriptors and sampler.\n");

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

	// Destroy G-buffer descriptor layout:
	if (descriptors->g_buffer_descriptor_layout != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorSetLayout(device->logical_device,
			descriptors->g_buffer_descriptor_layout, NULL);
	}

	// Free memory for G-buffer descriptors:
	if (descriptors->g_buffer_descriptors)
	{
		free(descriptors->g_buffer_descriptors);
	}

	// Destroy sampler:
	if (descriptors->sampler != VK_NULL_HANDLE)
	{
		vkDestroySampler(device->logical_device, descriptors->sampler, NULL);
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

// Create sampler:
int create_sampler(FracRenderVulkanDevice *device, FracRenderVulkanDescriptors *descriptors)
{
	// Define sampler creation info:
	VkSamplerCreateInfo sampler_info;
	sampler_info.sType			= VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler_info.pNext			= NULL;
	sampler_info.flags			= 0;
	sampler_info.magFilter			= VK_FILTER_LINEAR;
	sampler_info.minFilter			= VK_FILTER_LINEAR;
	sampler_info.mipmapMode			= VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler_info.addressModeU		= VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_info.addressModeV		= VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_info.addressModeW		= VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_info.mipLodBias			= 0.f;
	sampler_info.anisotropyEnable		= VK_FALSE;
	sampler_info.maxAnisotropy		= 0.f;
	sampler_info.compareEnable		= VK_FALSE;
	sampler_info.compareOp			= VK_COMPARE_OP_NEVER;
	sampler_info.minLod			= 0.f;
	sampler_info.maxLod			= VK_LOD_CLAMP_NONE;
	sampler_info.borderColor		= VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
	sampler_info.unnormalizedCoordinates	= VK_FALSE;

	// Create the sampler:
	if (vkCreateSampler(device->logical_device, &sampler_info, NULL,
				&descriptors->sampler) != VK_SUCCESS)
	{
		fprintf(stderr, "Error, Unable to create the sampler!\n");
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

// Create scene descriptor layout:
int create_scene_descriptor_layout(FracRenderVulkanDevice *device,
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

	return 0;
}

// Create scene descriptor:
int create_scene_descriptor(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors)
{
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

// Create G-buffer descriptor layout:
int create_g_buffer_descriptor_layout(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors)
{
	// Create array of descriptor set layout bindings:
	VkDescriptorSetLayoutBinding bindings[1];
	bindings[0].binding		= 0;
	bindings[0].descriptorType	= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	bindings[0].descriptorCount	= 1;
	bindings[0].stageFlags		= VK_SHADER_STAGE_FRAGMENT_BIT;
	bindings[0].pImmutableSamplers	= NULL;

	// Create descriptor set layout:
	VkDescriptorSetLayoutCreateInfo layout_info;
	layout_info.sType		= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.pNext		= NULL;
	layout_info.flags		= 0;
	layout_info.bindingCount	= 1;
	layout_info.pBindings		= bindings;

	if (vkCreateDescriptorSetLayout(device->logical_device, &layout_info,
		NULL, &descriptors->g_buffer_descriptor_layout) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to create G-buffer descriptor set layout!\n");
		return -1;
	}

	return 0;
}

// Create G-buffer descriptors:
int create_g_buffer_descriptors(FracRenderVulkanDevice *device,
	FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanDescriptors *descriptors)
{
	// Allocate memory for descriptors. Free in destroy_vulkan_descriptors:
	descriptors->g_buffer_descriptors = malloc(descriptors->num_g_buffer_descriptors *
								sizeof(VkDescriptorSet));

	// Initialize descriptors to VK_NULL_HANDLE:
	for (uint32_t i = 0; i < descriptors->num_g_buffer_descriptors; i++)
	{
		descriptors->g_buffer_descriptors[i] = VK_NULL_HANDLE;
	}

	// Loop through G-buffer images:
	for (uint32_t i = 0; i < descriptors->num_g_buffer_descriptors; i++)
	{
		// Allocate descriptor set:
		VkDescriptorSetAllocateInfo allocate_info;
		allocate_info.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocate_info.pNext			= NULL;
		allocate_info.descriptorPool		= descriptors->descriptor_pool;
		allocate_info.descriptorSetCount	= 1;
		allocate_info.pSetLayouts		= &descriptors->g_buffer_descriptor_layout;

		if (vkAllocateDescriptorSets(device->logical_device, &allocate_info,
				&descriptors->g_buffer_descriptors[i]) != VK_SUCCESS)
		{
			fprintf(stderr, "Error: Unable to allocate G-buffer "
						"descriptor set %d!\n", i);
			return -1;
		}

		// Define texture and sampler info:
		VkDescriptorImageInfo image_info;
		image_info.sampler	= descriptors->sampler;
		image_info.imageView	= framebuffers->g_buffer_image_views[i];
		image_info.imageLayout	= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		// Create descriptor set writing info:
		VkWriteDescriptorSet descriptor_write[1];
		descriptor_write[0].sType		= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptor_write[0].pNext		= NULL;
		descriptor_write[0].dstSet		= descriptors->g_buffer_descriptors[i];
		descriptor_write[0].dstBinding		= 0;
		descriptor_write[0].dstArrayElement	= 0;
		descriptor_write[0].descriptorCount	= 1;
		descriptor_write[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptor_write[0].pImageInfo		= &image_info;
		descriptor_write[0].pBufferInfo		= NULL;
		descriptor_write[0].pTexelBufferView	= NULL;

		// Update the descriptor sets:
		vkUpdateDescriptorSets(device->logical_device, 1, descriptor_write, 0, NULL);
	}

	return 0;
}
