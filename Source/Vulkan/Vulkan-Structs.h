#ifndef FRACRENDER_VULKAN_STRUCTS_H
#define FRACRENDER_VULKAN_STRUCTS_H

// Library includes:
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

typedef struct {
	// Instance and window:
	VkInstance instance;
	GLFWwindow *window;
	VkSurfaceKHR surface;

	// Debug messenger:
	VkDebugUtilsMessengerEXT debug_messenger;
} FracRenderVulkanBase;

typedef struct {
	// Devices:
	VkPhysicalDevice physical_device;
	VkDevice logical_device;

	// Device extensions:
	uint32_t num_device_extensions;
	char *device_extensions[1];

	// Queue families:
	uint32_t graphics_family_index;
	VkQueue graphics_queue;
	uint32_t present_family_index;
	VkQueue present_queue;
} FracRenderVulkanDevice;

typedef struct {
	// Validation:
	uint32_t num_validation_layers;
	uint32_t num_validation_extensions;

#ifdef FRACRENDER_DEBUG
	char *validation_layers[1];
	char *validation_extensions[1];
#else
	char *validation_layers[0];
	char *validation_extensions[0];
#endif
} FracRenderVulkanValidation;

typedef struct {
	// Swapchain:
	VkSwapchainKHR swapchain;

	// Images:
	uint32_t num_swapchain_images;
	VkImage *swapchain_images;
	VkImageView *swapchain_image_views;

	// Format:
	VkFormat swapchain_format;
	VkExtent2D swapchain_extent;
} FracRenderVulkanSwapchain;

#endif
