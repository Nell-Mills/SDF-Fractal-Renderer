#include "Vulkan-Base.h"

// Initialize Vulkan environment and populate data structure:
int initialize_vulkan_base(FracRenderVulkanBase *base, FracRenderVulkanValidation *validation)
{
	/* Initialization level:
	 * 0 --> Nothing done yet.
	 * 1 --> GLFW initialized.
	 * 2 --> GLFW window created.
	 * 3 --> Vulkan KHR surface created.
	 * 4 --> Vulkan instance created.*/

	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Initializing Vulkan instance and window...\n");

	// Initialize GLFW and make sure Vulkan is supported:
	printf(" ---> Creating GLFW window and Vulkan surface.\n");
	if (create_glfw_window(base) != 0)
	{
		destroy_vulkan_base(base);
		return -1;
	}

	// Create Vulkan instance:
	printf(" ---> Creating the Vulkan instance.\n");
	if (create_vulkan_instance(base, validation) != 0)
	{
		destroy_vulkan_base(base);
		return -1;
	}

	// Create KHR surface:
	printf(" ---> Creating the Vulkan surface.\n");
	if (create_KHR_surface(base) != 0)
	{
		destroy_vulkan_base(base);
		return -1;
	}

#ifdef FRACRENDER_DEBUG
	// Create debug messenger if enabled:
	printf(" ---> Creating the debug messenger.\n");
	if (create_debug_messenger(base) != 0)
	{
		destroy_vulkan_base(base);
		return -1;
	}
#endif

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	return 0;
}

// Destroy base Vulkan environment:
int destroy_vulkan_base(FracRenderVulkanBase *base)
{
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

	return 0;
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
	VkApplicationInfo applicationInfo = {
		.sType			= VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext			= NULL,
		.pApplicationName	= "Fractal Renderer",
		.applicationVersion	= VK_MAKE_VERSION(1, 0, 0),
		.pEngineName		= "None",
		.engineVersion		= VK_MAKE_VERSION(1, 0, 0),
		.apiVersion		= VK_MAKE_API_VERSION(0, 1, 3, 0) // Version 1.3.
	};

	// Get GLFW extensions:
	uint32_t num_glfw_extensions = 0;
	char const **glfw_extensions = glfwGetRequiredInstanceExtensions(&num_glfw_extensions);

	// Gather all extension names:
	int max_name_length = 64;
	uint32_t extension_count = 0;
	extension_count += validation->num_validation_extensions;
	extension_count += num_glfw_extensions;
	char **extensions = malloc(extension_count * sizeof(char *));

	int n = 0;
	for (int i = n; i < (n + validation->num_validation_extensions); i++)
	{
		extensions[i] = malloc(max_name_length * sizeof(char));
		strcpy(extensions[i], validation->validation_extensions[i - n]);
	}
	n += validation->num_validation_extensions;
	for (int i = n; i < (n + num_glfw_extensions); i++)
	{
		extensions[i] = malloc(max_name_length * sizeof(char));
		strcpy(extensions[i], glfw_extensions[i - n]);
	}

	// Define instance creation info:
	VkInstanceCreateInfo instanceCreateInfo = {
		.sType				= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext				= NULL,
		.flags				= 0,
		.pApplicationInfo		= &applicationInfo,
		.enabledLayerCount		= validation->num_validation_layers,
		.ppEnabledLayerNames		= (const char **)validation->
								validation_layers,
		.enabledExtensionCount		= extension_count,
		.ppEnabledExtensionNames	= (const char **)extensions
	};

	// Create the instance:
	base->instance = VK_NULL_HANDLE;
	if (vkCreateInstance(&instanceCreateInfo, NULL, &base->instance) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to create the Vulkan instance!\n");
		base->instance = VK_NULL_HANDLE;
		return -1;
	}
	if (base->instance == VK_NULL_HANDLE)
	{
		fprintf(stderr, "Error: Failed to create the Vulkan instance!\n");
		return -1;
	}

	// Free memory:
	for (int i = 0; i < extension_count; i++)
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
	char *result = "Temp";

	return result;
}

// Get message type char array value:
char *get_message_type_char(VkDebugUtilsMessageTypeFlagsEXT type)
{
	char *result = "Temp";

	return result;
}
