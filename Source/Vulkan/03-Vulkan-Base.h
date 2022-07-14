#ifndef FRACRENDER_VULKAN_BASE_H
#define FRACRENDER_VULKAN_BASE_H

/*************************************************************************
 * To set up and destroy the Vulkan environment and window for rendering *
 *************************************************************************/

// Library includes:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"
#include "01-Vulkan-Structs.h"

// GCC won't recognise this function during compilation for some reason so declare it here:
extern GLFWAPI VkResult glfwCreateWindowSurface(VkInstance instance, GLFWwindow* window,
			const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface);

/***********************
 * Function Prototypes *
************************/

// Initialize Vulkan environment and populate data structure:
int initialize_vulkan_base(FracRenderVulkanBase *base, FracRenderVulkanValidation *validation);

// Destroy base Vulkan environment:
void destroy_vulkan_base(FracRenderVulkanBase *base);

// Create GLFW window:
int create_glfw_window(FracRenderVulkanBase *base);

// Create KHR surface:
int create_KHR_surface(FracRenderVulkanBase *base);

// Create Vulkan instance:
int create_vulkan_instance(FracRenderVulkanBase *base, FracRenderVulkanValidation *validation);

// Create debug messenger:
int create_debug_messenger(FracRenderVulkanBase *base);

// Debug callback function:
VKAPI_ATTR VkBool32 VKAPI_CALL debug_util_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT severity,
	VkDebugUtilsMessageTypeFlagsEXT type,
	VkDebugUtilsMessengerCallbackDataEXT const *data,
	void *user_pointer);

// Get severity flag char array value:
char *get_severity_char(VkDebugUtilsMessageSeverityFlagBitsEXT severity);

// Get message type char array value:
char *get_message_type_char(VkDebugUtilsMessageTypeFlagsEXT type);

#endif
