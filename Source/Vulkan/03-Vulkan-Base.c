#include "03-Vulkan-Base.h"

// Initialize Vulkan environment and populate data structure:
int initialize_vulkan_base(FracRenderVulkanBase *base, FracRenderVulkanValidation *validation)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Initializing Vulkan instance and window...\n");

	// Initialize GLFW and make sure Vulkan is supported:
	printf(" ---> Creating GLFW window and Vulkan surface.\n");
	if (create_glfw_window(base) != 0)
	{
		return -1;
	}

	// Create Vulkan instance:
	printf(" ---> Creating the Vulkan instance.\n");
	if (create_vulkan_instance(base, validation) != 0)
	{
		return -1;
	}

	// Create KHR surface:
	printf(" ---> Creating the Vulkan surface.\n");
	if (create_KHR_surface(base) != 0)
	{
		return -1;
	}

#ifdef FRACRENDER_DEBUG
	// Create debug messenger if enabled:
	printf(" ---> Creating the debug messenger.\n");
	if (create_debug_messenger(base) != 0)
	{
		return -1;
	}
#endif

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	return 0;
}

// Destroy base Vulkan environment:
void destroy_vulkan_base(FracRenderVulkanBase *base)
{
	printf(" ---> Destroying Vulkan instance and window.\n");

	// Destroy debug messenger:
	if (base->debug_messenger != VK_NULL_HANDLE)
	{
		vkDestroyDebugUtilsMessengerEXT(base->instance, base->debug_messenger, NULL);
	}

	// Destroy surface:
	if (base->surface != VK_NULL_HANDLE)
	{
		vkDestroySurfaceKHR(base->instance, base->surface, NULL);
	}

	// Destroy instance:
	if (base->instance != VK_NULL_HANDLE)
	{
		vkDestroyInstance(base->instance, NULL);
	}

	// Destroy GLFW window:
	if (base->window)
	{
		glfwDestroyWindow(base->window);
	}

	// Terminate GLFW:
	glfwTerminate();
}

// Create GLFW window:
int create_glfw_window(FracRenderVulkanBase *base)
{
	// Initialize GLFW:
	if (glfwInit() != GLFW_TRUE)
	{
		char const *error = NULL;
		glfwGetError(&error);
		fprintf(stderr, "Error: Failed to initialize GLFW! Error code: %s\n", error);
		return -1;
	}

	// Create the GLFW window:
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	base->window = glfwCreateWindow(1280, 720, "Fractal Renderer", NULL, NULL);
	if (!base->window)
	{
		char const *error = NULL;
		glfwGetError(&error);
		fprintf(stderr, "Error: Failed to create GLFW window! Error code: %s\n", error);
		return -1;
	}

	return 0;
}

// Create KHR surface:
int create_KHR_surface(FracRenderVulkanBase *base)
{
	if (glfwCreateWindowSurface(base->instance, base->window, NULL,
						&base->surface) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to create KHR surface!\n");
		return -1;
	}

	return 0;
}

// Create Vulkan instance:
int create_vulkan_instance(FracRenderVulkanBase *base, FracRenderVulkanValidation *validation)
{
	// Define application info:
	VkApplicationInfo application_info;
	memset(&application_info, 0, sizeof(VkApplicationInfo));
	application_info.sType			= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.pNext			= NULL;
	application_info.pApplicationName	= "Fractal Renderer";
	application_info.applicationVersion	= VK_MAKE_VERSION(1, 0, 0);
	application_info.pEngineName		= "None";
	application_info.engineVersion		= VK_MAKE_VERSION(1, 0, 0);
	application_info.apiVersion		= VK_MAKE_API_VERSION(0, 1, 3, 0); // Version 1.3.

	// Get GLFW extensions:
	uint32_t num_glfw_extensions = 0;
	char const **glfw_extensions = glfwGetRequiredInstanceExtensions(&num_glfw_extensions);

	// Gather all extension names:
	int max_name_length = 128;
	uint32_t extension_count = 0;
	extension_count += validation->num_validation_extensions;
	extension_count += num_glfw_extensions;
	char **extensions = malloc(extension_count * sizeof(char *));

	int n = 0;
	for (uint32_t i = n; i < (n + validation->num_validation_extensions); i++)
	{
		extensions[i] = malloc(max_name_length * sizeof(char));
		strcpy(extensions[i], validation->validation_extensions[i - n]);
	}
	n += validation->num_validation_extensions;
	for (uint32_t i = n; i < (n + num_glfw_extensions); i++)
	{
		extensions[i] = malloc(max_name_length * sizeof(char));
		strcpy(extensions[i], glfw_extensions[i - n]);
	}

	// Define instance creation info:
	VkInstanceCreateInfo instance_create_info;
	memset(&instance_create_info, 0, sizeof(VkInstanceCreateInfo));
	instance_create_info.sType			= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pNext			= NULL;
	instance_create_info.flags			= 0;
	instance_create_info.pApplicationInfo		= &application_info;
	instance_create_info.enabledLayerCount		= validation->num_validation_layers;
	instance_create_info.ppEnabledLayerNames	= (const char **)validation->
									validation_layers;
	instance_create_info.enabledExtensionCount	= extension_count;
	instance_create_info.ppEnabledExtensionNames	= (const char **)extensions;

	// Create the instance:
	if (vkCreateInstance(&instance_create_info, NULL, &base->instance) != VK_SUCCESS)
	{
		// Free memory:
		for (uint32_t i = 0; i < extension_count; i++)
		{
			free(extensions[i]);
		}
		free(extensions);

		fprintf(stderr, "Error: Failed to create the Vulkan instance!\n");
		base->instance = VK_NULL_HANDLE;
		return -1;
	}
	if (base->instance == VK_NULL_HANDLE)
	{
		// Free memory:
		for (uint32_t i = 0; i < extension_count; i++)
		{
			free(extensions[i]);
		}
		free(extensions);

		fprintf(stderr, "Error: Failed to create the Vulkan instance!\n");
		return -1;
	}

	// Free memory:
	for (uint32_t i = 0; i < extension_count; i++)
	{
		free(extensions[i]);
	}
	free(extensions);

	// Load the rest of the Vulkan API:
	volkLoadInstance(base->instance);

	return 0;
}

// Create debug messenger:
int create_debug_messenger(FracRenderVulkanBase *base)
{
	// Define messenger creation information:
	VkDebugUtilsMessengerCreateInfoEXT debug_info;
	memset(&debug_info, 0, sizeof(VkDebugUtilsMessengerCreateInfoEXT));
	debug_info.sType			= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debug_info.pNext			= NULL;
	debug_info.flags			= 0;

	debug_info.messageSeverity	= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
					VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	debug_info.messageType		= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
					VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
					VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	debug_info.pfnUserCallback	= &debug_util_callback;
	debug_info.pUserData		= NULL;

	if (vkCreateDebugUtilsMessengerEXT(base->instance, &debug_info, NULL, &base->debug_messenger) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to create debug messenger!\n");
		return -1;
	}

	return 0;
}

// Debug callback function:
VKAPI_ATTR VkBool32 VKAPI_CALL debug_util_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT severity,
	VkDebugUtilsMessageTypeFlagsEXT type,
	VkDebugUtilsMessengerCallbackDataEXT const *data,
	void *user_pointer)
{
	char *severity_char = get_severity_char(severity);
	char *type_char = get_message_type_char(type);

	fprintf(stderr, "%s (%s): %s (%d)\n%s\n--\n",
		severity_char, type_char, data->pMessageIdName,
		data->messageIdNumber, data->pMessage);

	return VK_FALSE;
}

// Get severity flag char array value:
char *get_severity_char(VkDebugUtilsMessageSeverityFlagBitsEXT severity)
{
	if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
	{ return "SEVERITY_VERBOSE"; }
	else if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
	{ return "SEVERITY_INFO"; }
	else if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{ return "SEVERITY_WARNING"; }
	else if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{ return "SEVERITY_ERROR"; }
	else
	{ return "UNKNOWN SEVERITY TYPE"; }
}

// Get message type char array value:
char *get_message_type_char(VkDebugUtilsMessageTypeFlagsEXT type)
{
	if (type == VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
	{ return "GENERAL"; }
	if (type == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
	{ return "VALIDATION"; }
	if (type == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
	{ return "PERFORMANCE"; }
	else
	{ return "UNKNOWN MESSAGE TYPE"; }
}
