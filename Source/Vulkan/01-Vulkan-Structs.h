#ifndef FRACRENDER_VULKAN_STRUCTS_H
#define FRACRENDER_VULKAN_STRUCTS_H

/********************************************************
 * Structures for storing the entire Vulkan environment *
 ********************************************************/

// Library includes:
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"
#include "../Utility/Vectors.h"

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

typedef struct {
	// Descriptor pool:
	VkDescriptorPool descriptor_pool;

	// Sampler:
	VkSampler sampler;

	// Scene descriptor:
	VkDescriptorSetLayout scene_descriptor_layout;
	VkDescriptorSet scene_descriptor;
	VkBuffer scene_buffer;
	VkDeviceMemory scene_memory;

	// G-buffer descriptors:
	uint32_t num_g_buffer_descriptors;
	VkDescriptorSetLayout g_buffer_descriptor_layout;
	VkDescriptorSet *g_buffer_descriptors;

	// 3D SDF descriptor:
	VkDescriptorSetLayout sdf_3d_descriptor_layout;
	VkDescriptorSet sdf_3d_descriptor;
	VkBuffer sdf_3d_buffer;
	VkDeviceMemory sdf_3d_memory;

	// Temporal Cache descriptor:
	VkDescriptorSetLayout temporal_cache_descriptor_layout;
	VkDescriptorSet temporal_cache_descriptor;
} FracRenderVulkanDescriptors;

typedef struct {
	// Pipeline layouts:
	VkPipelineLayout geometry_pipeline_layout;
	VkPipelineLayout colour_pipeline_layout;

	// Pipelines:
	VkPipeline geometry_pipeline;
	VkPipeline colour_pipeline;

	// Render passes:
	VkRenderPass geometry_render_pass;
	VkRenderPass colour_render_pass;

	// Shader modules:
	VkShaderModule geometry_vertex_shader;
	VkShaderModule geometry_fragment_shader;
	VkShaderModule colour_vertex_shader;
	VkShaderModule colour_fragment_shader;

	// Shader paths:
	const char *geometry_vertex_shader_path;
	const char *geometry_fragment_shader_path;
	const char *colour_vertex_shader_path;
	const char *colour_fragment_shader_path;
} FracRenderVulkanPipeline;

typedef struct {
	// Framebuffers:
	VkFramebuffer *framebuffers;

	// G-buffer:
	VkFramebuffer g_buffer;
	uint32_t num_g_buffer_images;
	VkImage *g_buffer_images;
	VkDeviceMemory *g_buffer_image_memory;
	VkImageView *g_buffer_image_views;
	VkFormat *g_buffer_formats;

	// Temporal Cache:
	VkImage temporal_cache_image;
	VkDeviceMemory temporal_cache_memory;
	VkImageView temporal_cache_image_view;
	VkFormat temporal_cache_format;
} FracRenderVulkanFramebuffers;

typedef struct {
	// Command pool:
	VkCommandPool command_pool;

	// Command buffers:
	VkCommandBuffer *command_buffers;

	// Fences:
	VkFence *fences;

	// Semaphores:
	VkSemaphore image_available;
	VkSemaphore render_finished;
} FracRenderVulkanCommands;

typedef struct {
	// Query pool:
	VkQueryPool query_pool;

	// Device timestamp period:
	float timestamp_period;
} FracRenderVulkanPerformance;

typedef struct {
	// Axes in eye coordinate system:
	FracRenderVector3 plane_centre; float pad_0;
	FracRenderVector3 x_axis; float pad_1;
	FracRenderVector3 y_axis; float pad_2;

	// Eye position:
	FracRenderVector3 eye_position; float pad_3;

	// 3D SDF information:
	FracRenderVector3 sdf_3d_centre;
	float sdf_3d_size;
	uint32_t sdf_3d_levels;

	// Aspect ratio:
	float aspect_ratio;

	// Fractal parameter:
	float fractal_parameter;

	// View distance:
	float view_distance;
} FracRenderVulkanSceneUniform;

#endif
