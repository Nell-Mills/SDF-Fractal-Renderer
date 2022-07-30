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
#include "../SDF/SDF-3D.h"

/***********************
 * Function Prototypes *
************************/

// Create Vulkan descriptor layouts:
int initialize_vulkan_descriptor_layouts(FracRenderVulkanDevice *device,
	FracRenderVulkanDescriptors *descriptors, FracRenderSDF3D *sdf_3d, int sdf_type);

// Create Vulkan descriptors (after creating the framebuffers):
int initialize_vulkan_descriptors(FracRenderVulkanDevice *device,
	FracRenderVulkanFramebuffers *framebuffers,
	FracRenderVulkanDescriptors *descriptors, int sdf_type);

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

// Create buffer for 3D SDF:
int create_sdf_3d_buffer(FracRenderVulkanDevice *device, FracRenderVulkanDescriptors *descriptors,
									FracRenderSDF3D *sdf_3d);

// Create 3D SDF descriptor set layout:
int create_sdf_3d_descriptor_layout(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors);

// Create 3D SDF descriptor:
int create_sdf_3d_descriptor(FracRenderVulkanDevice *device,
		FracRenderVulkanDescriptors *descriptors);

// Copy data into 3D SDF buffer:
int copy_sdf_3d_data(FracRenderVulkanDevice *device, FracRenderVulkanDescriptors *descriptors,
				FracRenderVulkanCommands *commands, FracRenderSDF3D *sdf_3d);

// Create 2D SDF descriptor set layout:
int create_sdf_2d_descriptor_layout(FracRenderVulkanDevice *device,
			FracRenderVulkanDescriptors *descriptors);

// Create 2D SDF descriptor:
int create_sdf_2d_descriptor(FracRenderVulkanDevice *device,
		FracRenderVulkanDescriptors *descriptors);

#endif
