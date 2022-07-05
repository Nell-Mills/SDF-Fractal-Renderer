#ifndef FRACRENDER_VULKAN_STRUCTS_H
#define FRACRENDER_VULKAN_STRUCTS_H

typedef struct {
	int max_level;
	VkInstance vulkan_instance;
	GLFWwindow *window;
	VkSurfaceKHR present_surface;
} FracRenderVulkanBase;

typedef struct {
	int max_level;

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
	int max_level;

	// Validation:
	int enable_validation; // 0 = on, -1 = off.
	uint32_t num_validation_layers;
	char *validation_layers[1];

	// Debugging:
	VkDebugUtilsMessengerEXT debug_messenger;
} FracRenderVulkanValidation;

typedef struct {
	int max_level;

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
