#ifndef FRACRENDER_VULKAN_DESCRIPTORS_H
#define FRACRENDER_VULKAN_DESCRIPTORS_H

/************************************************************
 * To set up and destroy the Vulkan descriptors and sampler *
 ************************************************************/

// Library includes:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"
#include "01-Vulkan-Structs.h"

/***********************
 * Function Prototypes *
************************/

// Create Vulkan descriptor layouts:
int initialize_vulkan_descriptor_layouts(FracRenderVulkanDevice *device,
				FracRenderVulkanDescriptors *descriptors);

// Create Vulkan descriptors (after creating the framebuffers):
int initialize_vulkan_descriptors(FracRenderVulkanDevice *device,
	FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanDescriptors *descriptors);

// Destroy Vulkan descriptors:
void destroy_vulkan_descriptors(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors);

// Create descriptor pool:
int create_descriptor_pool(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors);

// Create sampler:
int create_sampler(FracRenderVulkanDevice *device, FracRenderVulkanDescriptors *descriptors);

// Create scene buffer:
int create_scene_buffer(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors);

// Create scene descriptor layout:
int create_scene_descriptor_layout(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors);

// Create scene descriptor:
int create_scene_descriptor(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors);

// Create G-buffer descriptor layout:
int create_g_buffer_descriptor_layout(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors);

// Create G-buffer descriptors:
int create_g_buffer_descriptors(FracRenderVulkanDevice *device,
	FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanDescriptors *descriptors);

// Update G-buffer descriptors:
void update_vulkan_g_buffer_descriptors(FracRenderVulkanDevice *device,
	FracRenderVulkanFramebuffers *framebuffers, FracRenderVulkanDescriptors *descriptors);

#endif
