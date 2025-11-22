#include "07-Vulkan-Pipeline.h"

// Create Vulkan pipelines and render passes:
int initialize_vulkan_pipeline(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanDescriptors *descriptors,
	FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanPipeline *pipeline,
	int optimize)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Initializing Vulkan pipelines, render passes and shader modules...\n");

	// Load the shaders:
	printf(" ---> Loading shaders.\n");
	if (load_shaders(device, pipeline) != 0)
	{
		return -1;
	}

	// Create geometry render pass:
	printf(" ---> Creating geometry render pass.\n");
	if (create_geometry_render_pass(device, framebuffers, pipeline, optimize) != 0)
	{
		return -1;
	}

	// Create geometry pipeline layout:
	printf(" ---> Creating geometry pipeline layout.\n");
	if (create_pipeline_layout(device, descriptors, pipeline, 0, optimize) != 0)
	{
		return -1;
	}

	// Create geometry pipeline:
	printf(" ---> Creating geometry pipeline.\n");
	if (create_pipeline(device, swapchain, pipeline, 0, optimize) != 0)
	{
		return -1;
	}

	// Create colour render pass:
	printf(" ---> Creating colour render pass.\n");
	if (create_colour_render_pass(device, swapchain, pipeline) != 0)
	{
		return -1;
	}

	// Create colour pipeline layout:
	printf(" ---> Creating colour pipeline layout.\n");
	if (create_pipeline_layout(device, descriptors, pipeline, 1, optimize) != 0)
	{
		return -1;
	}

	// Create colour pipeline:
	printf(" ---> Creating colour pipeline.\n");
	if (create_pipeline(device, swapchain, pipeline, 1, optimize) != 0)
	{
		return -1;
	}

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	return 0;
}

// Destroy Vulkan pipeline structure:
void destroy_vulkan_pipeline(FracRenderVulkanDevice *device, FracRenderVulkanPipeline *pipeline)
{
	printf(" ---> Destroying Vulkan pipelines, render passes and shader modules.\n");

	// Destroy geometry pipeline and layout:
	if (pipeline->geometry_pipeline != VK_NULL_HANDLE)
	{
		vkDestroyPipeline(device->logical_device, pipeline->geometry_pipeline, NULL);
	}
	if (pipeline->geometry_pipeline_layout != VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(device->logical_device,
			pipeline->geometry_pipeline_layout, NULL);
	}

	// Destroy colour pipeline and layout:
	if (pipeline->colour_pipeline != VK_NULL_HANDLE)
	{
		vkDestroyPipeline(device->logical_device, pipeline->colour_pipeline, NULL);
	}
	if (pipeline->colour_pipeline_layout != VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(device->logical_device,
			pipeline->colour_pipeline_layout, NULL);
	}

	// Destroy render passes:
	if (pipeline->geometry_render_pass != VK_NULL_HANDLE)
	{
		vkDestroyRenderPass(device->logical_device, pipeline->geometry_render_pass, NULL);
	}
	if (pipeline->colour_render_pass != VK_NULL_HANDLE)
	{
		vkDestroyRenderPass(device->logical_device, pipeline->colour_render_pass, NULL);
	}

	// Destroy shader modules:
	if (pipeline->geometry_vertex_shader != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(device->logical_device,
			pipeline->geometry_vertex_shader, NULL);
	}
	if (pipeline->geometry_fragment_shader != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(device->logical_device,
			pipeline->geometry_fragment_shader, NULL);
	}
	if (pipeline->colour_vertex_shader != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(device->logical_device,
			pipeline->colour_vertex_shader, NULL);
	}
	if (pipeline->colour_fragment_shader != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(device->logical_device,
			pipeline->colour_fragment_shader, NULL);
	}
}

// Load shaders:
int load_shaders(FracRenderVulkanDevice *device, FracRenderVulkanPipeline *pipeline)
{
	// Load the shader modules:
	pipeline->geometry_vertex_shader = load_shader_module(device,
				pipeline->geometry_vertex_shader_path);
	if (pipeline->geometry_vertex_shader == VK_NULL_HANDLE)
	{
		return -1;
	}

	pipeline->geometry_fragment_shader = load_shader_module(device,
				pipeline->geometry_fragment_shader_path);
	if (pipeline->geometry_fragment_shader == VK_NULL_HANDLE)
	{
		return -1;
	}

	pipeline->colour_vertex_shader = load_shader_module(device,
				pipeline->colour_vertex_shader_path);
	if (pipeline->colour_vertex_shader == VK_NULL_HANDLE)
	{
		return -1;
	}

	pipeline->colour_fragment_shader = load_shader_module(device,
				pipeline->colour_fragment_shader_path);
	if (pipeline->colour_fragment_shader == VK_NULL_HANDLE)
	{
		return -1;
	}

	return 0;
}

// Load shader module:
VkShaderModule load_shader_module(FracRenderVulkanDevice *device, const char *shader_path)
{
	// Open the file:
	FILE *shader_file = fopen(shader_path, "rb");
	if (shader_file == NULL)
	{
		fprintf(stderr, "Error: Unable to open shader file \"%s\"!\n", shader_path);
		return VK_NULL_HANDLE;
	}

	// Get the file's size:
	fseek(shader_file, 0, SEEK_END);
	size_t file_size = (size_t)(ftell(shader_file));
	fseek(shader_file, 0, SEEK_SET);

	// Check that it's a multiple of 4 bytes:
	if ((file_size % 4) != 0)
	{
		fprintf(stderr, "Error: Shader file \"%s\" size is not a multiple of 4 bytes!\n",
										shader_path);
		return VK_NULL_HANDLE;
	}

	// Get the number of words (all 32-bit words, hence the above check):
	size_t num_words = file_size / 4;

	// Get the shader code itself:
	uint32_t *shader_code = malloc(num_words * sizeof(uint32_t));
	size_t progress = 0;
	while (progress != num_words)
	{
		size_t words_read = fread(shader_code + progress, sizeof(uint32_t),
						num_words + progress, shader_file);
		if (words_read == 0)
		{
			// Close the file:
			fclose(shader_file);

			// Free memory:
			free(shader_code);

			// No words read, throw error:
			fprintf(stderr, "Error: Cannot read shader file \"%s\"!\n", shader_path);
			return VK_NULL_HANDLE;
		}

		progress += words_read;
	}

	// Close the file:
	fclose(shader_file);

	// Create the shader module info:
	VkShaderModuleCreateInfo module_info;
	memset(&module_info, 0, sizeof(VkShaderModuleCreateInfo));
	module_info.sType	= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	module_info.pNext	= NULL;
	module_info.flags	= 0;
	module_info.codeSize	= file_size;
	module_info.pCode	= shader_code;

	// Create the shader module:
	VkShaderModule shader_module = VK_NULL_HANDLE;
	if (vkCreateShaderModule(device->logical_device, &module_info, NULL,
						&shader_module) != VK_SUCCESS)
	{
		// Free memory:
		free(shader_code);

		fprintf(stderr, "Error: Unable to create shader module from file \"%s\"!\n",
										shader_path);
		return VK_NULL_HANDLE;
	}

	// Free memory:
	free(shader_code);

	return shader_module;
}

// Create geometry render pass:
int create_geometry_render_pass(FracRenderVulkanDevice *device,
	FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanPipeline *pipeline,
	int optimize)
{
	// Create attachment descriptions:
	VkAttachmentDescription *attachments;
	attachments = malloc(framebuffers->num_g_buffer_images * sizeof(VkAttachmentDescription));
	memset(attachments, 0, framebuffers->num_g_buffer_images * sizeof(VkAttachmentDescription));

	// Position/iteration attachment:
	attachments[0].flags		= 0;
	attachments[0].format		= framebuffers->g_buffer_formats[0];
	attachments[0].samples		= VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp		= VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp		= VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_LOAD;
	attachments[0].stencilStoreOp	= VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout	= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	if (optimize == 1)
	{
		// Distance write attachment:
		attachments[1].flags		= 0;
		attachments[1].format		= framebuffers->g_buffer_formats[1];
		attachments[1].samples		= VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp		= VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp		= VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_LOAD;
		attachments[1].stencilStoreOp	= VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout	= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	}

	// Create subpass attachments:
	VkAttachmentReference *subpass_attachments;
	subpass_attachments = malloc(framebuffers->num_g_buffer_images *
					sizeof(VkAttachmentReference));
	memset(subpass_attachments, 0, framebuffers->num_g_buffer_images *
					sizeof(VkAttachmentReference));

	// Position/iteration attachment:
	subpass_attachments[0].attachment	= 0;	// Attachments[0].
	subpass_attachments[0].layout		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	if (optimize == 1)
	{
		// Distance write attachment:
		subpass_attachments[1].attachment	= 1;	// Attachments[1].
		subpass_attachments[1].layout		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	}

	// Create subpass:
	VkSubpassDescription subpasses[1];
	memset(subpasses, 0, 1 * sizeof(VkSubpassDescription));
	subpasses[0].flags			= 0;
	subpasses[0].pipelineBindPoint		= VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpasses[0].inputAttachmentCount	= 0;
	subpasses[0].pInputAttachments		= NULL;
	subpasses[0].colorAttachmentCount	= framebuffers->num_g_buffer_images;
	subpasses[0].pColorAttachments		= subpass_attachments;
	subpasses[0].pResolveAttachments	= NULL;
	subpasses[0].pDepthStencilAttachment	= NULL;
	subpasses[0].preserveAttachmentCount	= 0;
	subpasses[0].pPreserveAttachments	= NULL;

	// Define render pass creation info:
	VkRenderPassCreateInfo pass_info;
	memset(&pass_info, 0, sizeof(VkRenderPassCreateInfo));
	pass_info.sType			= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	pass_info.pNext			= NULL;
	pass_info.flags			= 0;
	pass_info.attachmentCount	= framebuffers->num_g_buffer_images;
	pass_info.pAttachments		= attachments;
	pass_info.subpassCount		= 1;
	pass_info.pSubpasses		= subpasses;
	pass_info.dependencyCount	= 0;
	pass_info.pDependencies		= NULL;

	// Create render pass:
	if (vkCreateRenderPass(device->logical_device, &pass_info, NULL,
			&pipeline->geometry_render_pass) != VK_SUCCESS)
	{
		// Free memory:
		free(attachments);
		free(subpass_attachments);

		fprintf(stderr, "Error: Unable to create geometry render pass!\n");
		return -1;
	}

	// Free memory:
	free(attachments);
	free(subpass_attachments);

	return 0;
}

// Create colour render pass:
int create_colour_render_pass(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanPipeline *pipeline)
{
	// Create colour attachment description:
	VkAttachmentDescription attachments[1];
	memset(attachments, 0, 1 * sizeof(VkAttachmentDescription));
	attachments[0].flags		= 0;
	attachments[0].format		= swapchain->swapchain_format;
	attachments[0].samples		= VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp		= VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp		= VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_LOAD;
	attachments[0].stencilStoreOp	= VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout	= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// Create subpass attachments:
	VkAttachmentReference subpass_attachments[1];
	memset(subpass_attachments, 0, 1 * sizeof(VkAttachmentReference));

	// Position attachment:
	subpass_attachments[0].attachment	= 0;	// Attachments[0].
	subpass_attachments[0].layout		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// Create subpass:
	VkSubpassDescription subpasses[1];
	memset(subpasses, 0, 1 * sizeof(VkSubpassDescription));
	subpasses[0].flags			= 0;
	subpasses[0].pipelineBindPoint		= VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpasses[0].inputAttachmentCount	= 0;
	subpasses[0].pInputAttachments		= NULL;
	subpasses[0].colorAttachmentCount	= 1;
	subpasses[0].pColorAttachments		= subpass_attachments;
	subpasses[0].pResolveAttachments	= NULL;
	subpasses[0].pDepthStencilAttachment	= NULL;
	subpasses[0].preserveAttachmentCount	= 0;
	subpasses[0].pPreserveAttachments	= NULL;

	// Define render pass creation info:
	VkRenderPassCreateInfo pass_info;
	memset(&pass_info, 0, sizeof(VkRenderPassCreateInfo));
	pass_info.sType			= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	pass_info.pNext			= NULL;
	pass_info.flags			= 0;
	pass_info.attachmentCount	= 1;
	pass_info.pAttachments		= attachments;
	pass_info.subpassCount		= 1;
	pass_info.pSubpasses		= subpasses;
	pass_info.dependencyCount	= 0;
	pass_info.pDependencies		= NULL;

	// Create render pass:
	if (vkCreateRenderPass(device->logical_device, &pass_info, NULL,
			&pipeline->colour_render_pass) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to create colour render pass!\n");
		return -1;
	}

	return 0;
}

// Create pipeline layout:
int create_pipeline_layout(FracRenderVulkanDevice *device, FracRenderVulkanDescriptors *descriptors,
	FracRenderVulkanPipeline *pipeline, int pipe, int optimize)
{
	// Create an array of descriptor set layouts:
	uint32_t num_layouts;
	VkDescriptorSetLayout *layouts;
	if (pipe == 0)
	{
		// Geometry pipeline:
		if (optimize == 0)
		{
			num_layouts = 2;
			layouts = malloc(num_layouts * sizeof(VkDescriptorSetLayout));
			layouts[0] = descriptors->scene_descriptor_layout;
			layouts[1] = descriptors->sdf_3d_descriptor_layout;
		}
		else if (optimize == 1)
		{
			num_layouts = 2;
			layouts = malloc(num_layouts * sizeof(VkDescriptorSetLayout));
			layouts[0] = descriptors->scene_descriptor_layout;
			layouts[1] = descriptors->temporal_cache_descriptor_layout;
		}
		else
		{
			num_layouts = 1;
			layouts = malloc(num_layouts * sizeof(VkDescriptorSetLayout));
			layouts[0] = descriptors->scene_descriptor_layout;
		}
	}
	else
	{
		// Colour pipeline:
		num_layouts = 2;
		layouts = malloc(num_layouts * sizeof(VkDescriptorSetLayout));
		layouts[0] = descriptors->scene_descriptor_layout;
		layouts[1] = descriptors->g_buffer_descriptor_layout;
	}

	// Create the pipeline layout info:
	VkPipelineLayoutCreateInfo layout_info;
	memset(&layout_info, 0, sizeof(VkPipelineLayoutCreateInfo));
	layout_info.sType			= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layout_info.pNext			= NULL;
	layout_info.flags			= 0;
	layout_info.setLayoutCount		= num_layouts;
	layout_info.pSetLayouts			= layouts;
	layout_info.pushConstantRangeCount	= 0;
	layout_info.pPushConstantRanges		= NULL;

	// Create the pipeline layout:
	if (pipe == 0)
	{
		// Geometry pipeline:
		if (vkCreatePipelineLayout(device->logical_device, &layout_info, NULL,
				&pipeline->geometry_pipeline_layout) != VK_SUCCESS)
		{
			// Free memory:
			free(layouts);

			fprintf(stderr, "Error: Unable to create geometry pipeline layout!\n");
			return -1;
		}
	}
	else
	{
		// Colour pipeline:
		if (vkCreatePipelineLayout(device->logical_device, &layout_info, NULL,
				&pipeline->colour_pipeline_layout) != VK_SUCCESS)
		{
			// Free memory:
			free(layouts);

			fprintf(stderr, "Error: Unable to create colour pipeline layout!\n");
			return -1;
		}
	}

	// Free memory:
	free(layouts);

	return 0;
}

// Create pipeline:
int create_pipeline(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
				FracRenderVulkanPipeline *pipeline, int pipe, int optimize)
{
	// Define shader stages:
	VkPipelineShaderStageCreateInfo shader_stages[2];
	memset(shader_stages, 0, 2 * sizeof(VkPipelineShaderStageCreateInfo));

	// Vertex shader:
	shader_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader_stages[0].pNext			= NULL;
	shader_stages[0].flags			= 0;
	shader_stages[0].stage			= VK_SHADER_STAGE_VERTEX_BIT;
	shader_stages[0].pName			= "main";
	shader_stages[0].pSpecializationInfo	= NULL;

	// Fragment shader:
	shader_stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader_stages[1].pNext			= NULL;
	shader_stages[1].flags			= 0;
	shader_stages[1].stage			= VK_SHADER_STAGE_FRAGMENT_BIT;
	shader_stages[1].pName			= "main";
	shader_stages[1].pSpecializationInfo	= NULL;

	if (pipe == 0)
	{
		// Geometry pipeline:
		shader_stages[0].module = pipeline->geometry_vertex_shader;
		shader_stages[1].module = pipeline->geometry_fragment_shader;
	}
	else
	{
		// Colour pipeline:
		shader_stages[0].module = pipeline->colour_vertex_shader;
		shader_stages[1].module = pipeline->colour_fragment_shader;
	}

	// Define vertex input state:
	VkPipelineVertexInputStateCreateInfo input_info;
	memset(&input_info, 0, sizeof(VkPipelineVertexInputStateCreateInfo));
	input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	input_info.pNext				= NULL;
	input_info.flags				= 0;
	input_info.vertexBindingDescriptionCount	= 0;
	input_info.pVertexBindingDescriptions		= NULL;
	input_info.vertexAttributeDescriptionCount	= 0;
	input_info.pVertexAttributeDescriptions		= NULL;

	// Define input assembly state (which type of primitive to use):
	VkPipelineInputAssemblyStateCreateInfo assembly_info;
	memset(&assembly_info, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));
	assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	assembly_info.pNext			= NULL;
	assembly_info.flags			= 0;
	assembly_info.topology			= VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	assembly_info.primitiveRestartEnable	= VK_FALSE;

	// Define the viewport state:
	VkViewport viewport;
	memset(&viewport, 0, sizeof(VkViewport));
	viewport.x		= 0.f;
	viewport.y		= 0.f;
	viewport.width		= swapchain->swapchain_extent.width;
	viewport.height		= swapchain->swapchain_extent.height;
	viewport.minDepth	= 0.f;
	viewport.maxDepth	= 1.f;

	// Define the "scissor" (parts of the image to cut off):
	VkRect2D scissor;
	memset(&scissor, 0, sizeof(VkRect2D));
	scissor.offset.x	= 0;
	scissor.offset.y	= 0;
	scissor.extent.width	= swapchain->swapchain_extent.width;
	scissor.extent.height	= swapchain->swapchain_extent.height;

	// Create the viewport info structure:
	VkPipelineViewportStateCreateInfo viewport_info;
	memset(&viewport_info, 0, sizeof(VkPipelineViewportStateCreateInfo));
	viewport_info.sType		= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_info.pNext		= NULL;
	viewport_info.flags		= 0;
	viewport_info.viewportCount	= 1;
	viewport_info.pViewports	= &viewport;
	viewport_info.scissorCount	= 1;
	viewport_info.pScissors		= &scissor;

	// Define rasterization state:
	VkPipelineRasterizationStateCreateInfo raster_info;
	memset(&raster_info, 0, sizeof(VkPipelineRasterizationStateCreateInfo));
	raster_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	raster_info.pNext			= NULL;
	raster_info.flags			= 0;
	raster_info.depthClampEnable		= VK_FALSE;
	raster_info.rasterizerDiscardEnable	= VK_FALSE;
	raster_info.polygonMode			= VK_POLYGON_MODE_FILL;
	raster_info.cullMode			= VK_CULL_MODE_BACK_BIT;
	raster_info.frontFace			= VK_FRONT_FACE_CLOCKWISE;
	raster_info.depthBiasEnable		= VK_FALSE;
	raster_info.depthBiasConstantFactor	= 0.f;
	raster_info.depthBiasClamp		= 0.f;
	raster_info.depthBiasSlopeFactor	= 0.f;
	raster_info.lineWidth			= 1.f;

	// Define multisampling state (single sample):
	VkPipelineMultisampleStateCreateInfo sampling_info;
	memset(&sampling_info, 0, sizeof(VkPipelineMultisampleStateCreateInfo));
	sampling_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	sampling_info.pNext			= NULL;
	sampling_info.flags			= 0;
	sampling_info.rasterizationSamples	= VK_SAMPLE_COUNT_1_BIT;
	sampling_info.sampleShadingEnable	= VK_FALSE;
	sampling_info.minSampleShading		= 0.f;
	sampling_info.pSampleMask		= NULL;
	sampling_info.alphaToCoverageEnable	= VK_FALSE;
	sampling_info.alphaToOneEnable		= VK_FALSE;

	// Define blending state (one per attachment):
	uint32_t num_blend_states;
	VkPipelineColorBlendAttachmentState *blend_states;
	if (pipe == 0)
	{
		// Geometry pipeline:
		if (optimize == 1)
		{
			num_blend_states = 2;
		}
		else
		{
			num_blend_states = 1;
		}
	}
	else
	{
		// Colour pipeline:
		num_blend_states = 1;
	}

	blend_states = malloc(num_blend_states * sizeof(VkPipelineColorBlendAttachmentState));
	memset(blend_states, 0, num_blend_states * sizeof(VkPipelineColorBlendAttachmentState));

	for (uint32_t i = 0; i < num_blend_states; i++)
	{
		blend_states[i].blendEnable		= VK_FALSE;
		blend_states[i].srcColorBlendFactor	= VK_BLEND_FACTOR_ZERO;
		blend_states[i].dstColorBlendFactor	= VK_BLEND_FACTOR_ZERO;
		blend_states[i].colorBlendOp		= VK_BLEND_OP_ADD;
		blend_states[i].srcAlphaBlendFactor	= VK_BLEND_FACTOR_ZERO;
		blend_states[i].dstAlphaBlendFactor	= VK_BLEND_FACTOR_ZERO;
		blend_states[i].alphaBlendOp		= VK_BLEND_OP_ADD;
		blend_states[i].colorWriteMask		= VK_COLOR_COMPONENT_R_BIT |
							  VK_COLOR_COMPONENT_G_BIT |
							  VK_COLOR_COMPONENT_B_BIT |
							  VK_COLOR_COMPONENT_A_BIT;
	}

	// Create the blending state info structure:
	VkPipelineColorBlendStateCreateInfo blend_info;
	memset(&blend_info, 0, sizeof(VkPipelineColorBlendStateCreateInfo));
	blend_info.sType		= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	blend_info.pNext		= NULL;
	blend_info.flags		= 0;
	blend_info.logicOpEnable	= VK_FALSE;
	blend_info.logicOp		= VK_LOGIC_OP_CLEAR;
	blend_info.attachmentCount	= num_blend_states;
	blend_info.pAttachments		= blend_states;
	blend_info.blendConstants[0]	= 0.f;
	blend_info.blendConstants[1]	= 0.f;
	blend_info.blendConstants[2]	= 0.f;
	blend_info.blendConstants[3]	= 0.f;

	// Finally, create the pipeline creation structure:
	VkGraphicsPipelineCreateInfo pipeline_info;
	memset(&pipeline_info, 0, sizeof(VkGraphicsPipelineCreateInfo));
	pipeline_info.sType			= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.pNext			= NULL;
	pipeline_info.flags			= 0;

	pipeline_info.stageCount		= 2;
	pipeline_info.pStages			= shader_stages;

	pipeline_info.pVertexInputState		= &input_info;
	pipeline_info.pInputAssemblyState	= &assembly_info;
	pipeline_info.pTessellationState	= NULL;
	pipeline_info.pViewportState		= &viewport_info;
	pipeline_info.pRasterizationState	= &raster_info;
	pipeline_info.pMultisampleState		= &sampling_info;
	pipeline_info.pDepthStencilState	= NULL;
	pipeline_info.pColorBlendState		= &blend_info;
	pipeline_info.pDynamicState		= NULL;

	if (pipe == 0)
	{
		// Geometry pipeline:
		pipeline_info.layout		= pipeline->geometry_pipeline_layout;
		pipeline_info.renderPass	= pipeline->geometry_render_pass;
	}
	else
	{
		// Colour pipeline:
		pipeline_info.layout		= pipeline->colour_pipeline_layout;
		pipeline_info.renderPass	= pipeline->colour_render_pass;
	}

	pipeline_info.subpass			= 0;
	pipeline_info.basePipelineHandle	= VK_NULL_HANDLE;
	pipeline_info.basePipelineIndex		= 0;

	// Create the pipeline:
	if (pipe == 0)
	{
		// Geometry pipeline:
		if (vkCreateGraphicsPipelines(device->logical_device, VK_NULL_HANDLE, 1,
			&pipeline_info, NULL, &pipeline->geometry_pipeline) != VK_SUCCESS)
		{
			// Free memory:
			free(blend_states);

			fprintf(stderr, "Error: Unable to create geometry pipeline!\n");
			return -1;
		}
	}
	else
	{
		// Colour pipeline:
		if (vkCreateGraphicsPipelines(device->logical_device, VK_NULL_HANDLE, 1,
			&pipeline_info, NULL, &pipeline->colour_pipeline) != VK_SUCCESS)
		{
			// Free memory:
			free(blend_states);

			fprintf(stderr, "Error: Unable to create colour pipeline!\n");
			return -1;
		}
	}

	// Free memory:
	free(blend_states);

	return 0;
}

// Recreate render passes:
int recreate_vulkan_render_passes(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanFramebuffers *framebuffers,
	FracRenderVulkanPipeline *pipeline, int optimize)
{
	// Destroy old render passes:
	vkDestroyRenderPass(device->logical_device, pipeline->geometry_render_pass, NULL);
	vkDestroyRenderPass(device->logical_device, pipeline->colour_render_pass, NULL);

	pipeline->geometry_render_pass = VK_NULL_HANDLE;
	pipeline->colour_render_pass = VK_NULL_HANDLE;

	// Create new ones:
	if (create_geometry_render_pass(device, framebuffers, pipeline, optimize) != 0)
	{
		return -1;
	}
	if (create_colour_render_pass(device, swapchain, pipeline) != 0)
	{
		return -1;
	}

	return 0;
}

// Recreate pipelines:
int recreate_vulkan_pipelines(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
						FracRenderVulkanPipeline *pipeline, int optimize)
{
	// Destroy current pipelines:
	vkDestroyPipeline(device->logical_device, pipeline->geometry_pipeline, NULL);
	vkDestroyPipeline(device->logical_device, pipeline->colour_pipeline, NULL);

	pipeline->geometry_pipeline = VK_NULL_HANDLE;
	pipeline->colour_pipeline = VK_NULL_HANDLE;

	// Create new ones:
	if (create_pipeline(device, swapchain, pipeline, 0, optimize) != 0)
	{
		return -1;
	}
	if (create_pipeline(device, swapchain, pipeline, 1, optimize) != 0)
	{
		return -1;
	}

	return 0;
}
