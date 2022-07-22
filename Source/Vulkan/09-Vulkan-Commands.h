#ifndef FRACRENDER_VULKAN_COMMANDS_H
#define FRACRENDER_VULKAN_COMMANDS_H

/************************************************************************
 * To set up and destroy the Vulkan command pool, fences and semaphores *
 ************************************************************************/

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

// Create Vulkan command structure:
int initialize_vulkan_commands(FracRenderVulkanDevice *device,
	FracRenderVulkanSwapchain *swapchain, FracRenderVulkanCommands *commands);

// Destroy Vulkan command structure:
void destroy_vulkan_commands(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
							FracRenderVulkanCommands *commands);

// Create command pool:
int create_command_pool(FracRenderVulkanDevice *device, FracRenderVulkanCommands *commands);

// Create command buffers:
int create_command_buffers(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
							FracRenderVulkanCommands *commands);

// Create fences:
int create_fences(FracRenderVulkanDevice *device, FracRenderVulkanSwapchain *swapchain,
							FracRenderVulkanCommands *commands);

// Create semaphores:
int create_semaphores(FracRenderVulkanDevice *device, FracRenderVulkanCommands *commands);

#endif
