#include "10-Vulkan-Main.h"

// Update scene uniform:
void update_scene_uniform(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanSceneUniform *scene_uniform)
{
	// Get program state:
	FracRenderProgramState *program_state = (FracRenderProgramState *)
				glfwGetWindowUserPointer(base->window);

	// Get camera position:
	scene_uniform->camera_position = program_state->position;

	// Get axes in camera coordinate system:
	get_axes(program_state->position, program_state->front, program_state->up,
				&scene_uniform->x_axis, &scene_uniform->y_axis);

	// Get eye position (-10.f from camera position):
	scene_uniform->eye_position = initialize_vector_3(
		program_state->position.x - (10.f * program_state->front.x),
		program_state->position.y - (10.f * program_state->front.y),
		program_state->position.z - (10.f * program_state->front.z)
	);

	// Get screen aspect ratio:
	scene_uniform->aspect_ratio = (float)(swapchain->swapchain_extent.width)
				/ (float)(swapchain->swapchain_extent.height);

	// Scale x-axis by aspect ratio:
	scene_uniform->x_axis.x *= scene_uniform->aspect_ratio;
	scene_uniform->x_axis.y *= scene_uniform->aspect_ratio;
	scene_uniform->x_axis.z *= scene_uniform->aspect_ratio;
}

// Record commands:
int record_commands(FracRenderVulkanSwapchain *swapchain, FracRenderVulkanDescriptors *descriptors,
		FracRenderVulkanPipeline *pipeline, FracRenderVulkanFramebuffers *framebuffers,
		FracRenderVulkanCommands *commands, FracRenderVulkanSceneUniform *scene_uniform,
		uint32_t image_index)
{
	// Create command buffer begin info:
	VkCommandBufferBeginInfo begin_info;
	memset(&begin_info, 0, sizeof(VkCommandBufferBeginInfo));
	begin_info.sType		= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.pNext		= NULL;
	begin_info.flags		= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	begin_info.pInheritanceInfo	= NULL;

	// Begin recording commands:
	if (vkBeginCommandBuffer(commands->command_buffers[image_index],
					&begin_info) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to begin recording commands!\n");
		return -1;
	}

	// Upload scene uniform data. First put up a barrier:
	VkBufferMemoryBarrier buffer_barrier_1;
	memset(&buffer_barrier_1, 0, sizeof(VkBufferMemoryBarrier));
	buffer_barrier_1.sType			= VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	buffer_barrier_1.pNext			= NULL;
	buffer_barrier_1.srcAccessMask		= VK_ACCESS_UNIFORM_READ_BIT;
	buffer_barrier_1.dstAccessMask		= VK_ACCESS_TRANSFER_WRITE_BIT;
	buffer_barrier_1.srcQueueFamilyIndex	= VK_QUEUE_FAMILY_IGNORED;
	buffer_barrier_1.dstQueueFamilyIndex	= VK_QUEUE_FAMILY_IGNORED;
	buffer_barrier_1.buffer			= descriptors->scene_buffer;
	buffer_barrier_1.offset			= 0;
	buffer_barrier_1.size			= VK_WHOLE_SIZE;

	vkCmdPipelineBarrier(commands->command_buffers[image_index],
		VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 1, &buffer_barrier_1, 0, NULL);

	// Update buffer:
	vkCmdUpdateBuffer(commands->command_buffers[image_index], descriptors->scene_buffer,
				0, sizeof(FracRenderVulkanSceneUniform), scene_uniform);

	// Put up a final barrier:
	VkBufferMemoryBarrier buffer_barrier_2;
	memset(&buffer_barrier_2, 0, sizeof(VkBufferMemoryBarrier));
	buffer_barrier_2.sType			= VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	buffer_barrier_2.pNext			= NULL;
	buffer_barrier_2.srcAccessMask		= VK_ACCESS_TRANSFER_WRITE_BIT;
	buffer_barrier_2.dstAccessMask		= VK_ACCESS_UNIFORM_READ_BIT;
	buffer_barrier_2.srcQueueFamilyIndex	= VK_QUEUE_FAMILY_IGNORED;
	buffer_barrier_2.dstQueueFamilyIndex	= VK_QUEUE_FAMILY_IGNORED;
	buffer_barrier_2.buffer			= descriptors->scene_buffer;
	buffer_barrier_2.offset			= 0;
	buffer_barrier_2.size			= VK_WHOLE_SIZE;

	vkCmdPipelineBarrier(commands->command_buffers[image_index],
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT |
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 1, &buffer_barrier_2, 0, NULL);

	// Set G-buffer clear colour:
	VkClearValue geometry_clear_values[1];
	memset(&geometry_clear_values, 0, 1 * sizeof(VkClearValue));

	// Position:
	geometry_clear_values[0].color.float32[0] = 0.f;
	geometry_clear_values[0].color.float32[1] = 0.f;
	geometry_clear_values[0].color.float32[2] = 0.f;
	geometry_clear_values[0].color.float32[3] = 1.f;

	// Define render pass begin info:
	VkRenderPassBeginInfo geometry_pass_info;
	memset(&geometry_pass_info, 0, sizeof(VkRenderPassBeginInfo));
	geometry_pass_info.sType			= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	geometry_pass_info.pNext			= NULL;
	geometry_pass_info.renderPass			= pipeline->geometry_render_pass;
	geometry_pass_info.framebuffer			= framebuffers->g_buffer;
	geometry_pass_info.renderArea.offset.x		= 0;
	geometry_pass_info.renderArea.offset.y		= 0;
	geometry_pass_info.renderArea.extent.width	= swapchain->swapchain_extent.width;
	geometry_pass_info.renderArea.extent.height	= swapchain->swapchain_extent.height;
	geometry_pass_info.clearValueCount		= 1;
	geometry_pass_info.pClearValues			= geometry_clear_values;

	// Begin render pass:
	vkCmdBeginRenderPass(commands->command_buffers[image_index],
		&geometry_pass_info, VK_SUBPASS_CONTENTS_INLINE);

	// Bind geometry pipeline:
	vkCmdBindPipeline(commands->command_buffers[image_index],
		VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->geometry_pipeline);

	// Bind scene descriptor:
	vkCmdBindDescriptorSets(commands->command_buffers[image_index],
		VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->geometry_pipeline_layout,
		0, 1, &descriptors->scene_descriptor, 0, NULL);

	// Draw fullscreen triangle:
	vkCmdDraw(commands->command_buffers[image_index], 3, 1, 0, 0);

	// End the render pass:
	vkCmdEndRenderPass(commands->command_buffers[image_index]);

	// Wait here:
	for (uint32_t i = 0; i < framebuffers->num_g_buffer_images; i++)
	{
		VkImageMemoryBarrier image_barrier;
		memset(&image_barrier, 0, sizeof(VkImageMemoryBarrier));
		image_barrier.sType 			= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		image_barrier.pNext			= NULL;
		image_barrier.srcAccessMask		= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		image_barrier.dstAccessMask		= VK_ACCESS_SHADER_READ_BIT;
		image_barrier.oldLayout			= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		image_barrier.newLayout			= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		image_barrier.srcQueueFamilyIndex	= VK_QUEUE_FAMILY_IGNORED;
		image_barrier.dstQueueFamilyIndex	= VK_QUEUE_FAMILY_IGNORED;
		image_barrier.image			= framebuffers->g_buffer_images[i];

		image_barrier.subresourceRange.aspectMask	= VK_IMAGE_ASPECT_COLOR_BIT;
		image_barrier.subresourceRange.baseMipLevel	= 0;
		image_barrier.subresourceRange.levelCount	= 1;
		image_barrier.subresourceRange.baseArrayLayer	= 0;
		image_barrier.subresourceRange.layerCount	= 1;

		vkCmdPipelineBarrier(commands->command_buffers[image_index],
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL,
			0, NULL, 1, &image_barrier);
	}

	// Set framebuffer clear colour:
	VkClearValue colour_clear_values[1];
	memset(&colour_clear_values, 0, 1 * sizeof(VkClearValue));

	colour_clear_values[0].color.float32[0] = 0.f;
	colour_clear_values[0].color.float32[1] = 1.f;
	colour_clear_values[0].color.float32[2] = 1.f;
	colour_clear_values[0].color.float32[3] = 1.f;

	// Define render pass begin info:
	VkRenderPassBeginInfo colour_pass_info;
	memset(&colour_pass_info, 0, sizeof(VkRenderPassBeginInfo));
	colour_pass_info.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	colour_pass_info.pNext				= NULL;
	colour_pass_info.renderPass			= pipeline->colour_render_pass;
	colour_pass_info.framebuffer			= framebuffers->framebuffers[image_index];
	colour_pass_info.renderArea.offset.x		= 0;
	colour_pass_info.renderArea.offset.y		= 0;
	colour_pass_info.renderArea.extent.width	= swapchain->swapchain_extent.width;
	colour_pass_info.renderArea.extent.height	= swapchain->swapchain_extent.height;
	colour_pass_info.clearValueCount		= 1;
	colour_pass_info.pClearValues			= colour_clear_values;

	// Begin render pass:
	vkCmdBeginRenderPass(commands->command_buffers[image_index],
		&colour_pass_info, VK_SUBPASS_CONTENTS_INLINE);

	// Bind colour pipeline:
	vkCmdBindPipeline(commands->command_buffers[image_index],
		VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->colour_pipeline);

	// Bind scene descriptor:
	vkCmdBindDescriptorSets(commands->command_buffers[image_index],
		VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->colour_pipeline_layout,
		0, 1, &descriptors->scene_descriptor, 0, NULL);

	// Bind G-buffer descriptors:
	for (uint32_t i = 0; i < framebuffers->num_g_buffer_images; i++)
	{
		vkCmdBindDescriptorSets(commands->command_buffers[image_index],
			VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->colour_pipeline_layout,
			i + 1, 1, &descriptors->g_buffer_descriptors[i], 0, NULL);
	}

	// Draw fullscreen triangle:
	vkCmdDraw(commands->command_buffers[image_index], 3, 1, 0, 0);

	// End the render pass:
	vkCmdEndRenderPass(commands->command_buffers[image_index]);

	// End recording:
	if (vkEndCommandBuffer(commands->command_buffers[image_index]) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to end command recording!\n");
		return -1;
	}

	return 0;
}

// Submit commands:
int submit_commands(FracRenderVulkanDevice *device, FracRenderVulkanCommands *commands,
								uint32_t image_index)
{
	// Define which stage to wait at for the semaphore:
	VkPipelineStageFlags wait_stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	// Create command submission info:
	VkSubmitInfo submit_info;
	memset(&submit_info, 0, sizeof(VkSubmitInfo));
	submit_info.sType			= VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.pNext			= NULL;
	submit_info.waitSemaphoreCount		= 1;
	submit_info.pWaitSemaphores		= &commands->image_available;
	submit_info.pWaitDstStageMask		= &wait_stages;
	submit_info.commandBufferCount		= 1;
	submit_info.pCommandBuffers		= &commands->command_buffers[image_index];
	submit_info.signalSemaphoreCount	= 1;
	submit_info.pSignalSemaphores		= &commands->render_finished;

	// Submit commands:
	if (vkQueueSubmit(device->graphics_queue, 1, &submit_info,
		commands->fences[image_index]) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to submit commands!\n");
		return -1;
	}

	return 0;
}

// Present results:
int present_results(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
				FracRenderVulkanCommands *commands, uint32_t image_index)
{
	// Define presentation info:
	VkPresentInfoKHR present_info;
	memset(&present_info, 0, sizeof(VkPresentInfoKHR));
	present_info.sType		= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.pNext		= NULL;
	present_info.waitSemaphoreCount	= 1;
	present_info.pWaitSemaphores	= &commands->render_finished;
	present_info.swapchainCount	= 1;
	present_info.pSwapchains	= &swapchain->swapchain;
	present_info.pImageIndices	= &image_index;
	present_info.pResults		= NULL;

	// Present swapchain image:
	VkResult queue_result = vkQueuePresentKHR(device->present_queue, &present_info);

	if ((queue_result == VK_SUBOPTIMAL_KHR) || (queue_result == VK_ERROR_OUT_OF_DATE_KHR))
	{
		// Return 1, signifying that the swapchain needs recreating:
		return 1;
	}

	// Check for success:
	if (queue_result != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to present swapchain image %d!\n", image_index);
		return -1;
	}

	return 0;
}

// Print all Vulkan handles for debugging:
void print_vulkan_handles(FracRenderVulkanBase *base, FracRenderVulkanDevice *device,
		FracRenderVulkanValidation *validation, FracRenderVulkanSwapchain *swapchain,
		FracRenderVulkanDescriptors *descriptors, FracRenderVulkanPipeline *pipeline,
		FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanCommands *commands)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Vulkan Base:\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n");

	// Instance:
	if (base->instance == VK_NULL_HANDLE)
	{
		printf("Instance\t\t---> VK_NULL_HANDLE\n");
	}
	else
	{
		printf("Instance\t\t---> %p\n", base->instance);
	}

	// Window:
	if (base->window == NULL)
	{
		printf("Window\t\t\t---> NULL\n");
	}
	else
	{
		printf("Window\t\t\t---> %p\n", base->window);
	}

	// Surface:
	if (base->surface == VK_NULL_HANDLE)
	{
		printf("Surface\t\t\t---> VK_NULL_HANDLE\n");
	}
	else
	{
		printf("Surface\t\t\t---> %p\n", base->surface);
	}

	// Debug Messenger:
	if (base->debug_messenger == VK_NULL_HANDLE)
	{
		printf("Debug Messenger\t\t---> VK_NULL_HANDLE\n");
	}
	else
	{
		printf("Debug Messenger\t\t---> %p\n", base->debug_messenger);
	}

	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Vulkan Device:\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n");

	// Physical Device:
	if (device->physical_device == VK_NULL_HANDLE)
	{
		printf("Physical Device\t\t\t---> VK_NULL_HANDLE\n");
	}
	else
	{
		printf("Physical Device\t\t\t---> %p\n", device->physical_device);
	}

	// Logical Device:
	if (device->logical_device == VK_NULL_HANDLE)
	{
		printf("Logical Device\t\t\t---> VK_NULL_HANDLE\n");
	}
	else
	{
		printf("Logical Device\t\t\t---> %p\n", device->logical_device);
	}

	// Number of Device Extensions:
	printf("Number of Device Extensions\t---> %d\n", device->num_device_extensions);

	// Device Extensions:
	if (device->device_extensions == NULL)
	{
		printf("Device Extensions\t\t---> NULL\n");
	}
	else
	{
		printf("Device Extensions\t\t---> %p\n", device->device_extensions);
	}

	// Graphics Family Index:
	printf("Graphics Family Index\t\t---> %d\n", device->graphics_family_index);

	// Graphics Queue:
	if (device->graphics_queue == VK_NULL_HANDLE)
	{
		printf("Graphics Queue\t\t\t---> VK_NULL_HANDLE\n");
	}
	else
	{
		printf("Graphics Queue\t\t\t---> %p\n", device->graphics_queue);
	}

	// Present Family Index:
	printf("Present Family Index\t\t---> %d\n", device->present_family_index);

	// Present Queue:
	if (device->present_queue == VK_NULL_HANDLE)
	{
		printf("Present Queue\t\t\t---> VK_NULL_HANDLE\n");
	}
	else
	{
		printf("Present Queue\t\t\t---> %p\n", device->present_queue);
	}

	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
}
