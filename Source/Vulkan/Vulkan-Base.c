#include "Vulkan-Base.h"

// Initialize Vulkan environment and populate data structure:
int initialize_vulkan_base(FracRenderVulkanBase *base, FracRenderVulkanValidation *validation)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Initializing Vulkan environment...\n");

	// Variable to hold the initialization progress:
	int level = 0;

	// Use Volk to load the Vulkan API:
	printf(" ---> Loading Volk.\n");
	if (volkInitialize() != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to initialize Volk!\n");
		destroy_vulkan_base(base, level);
		return -1;
	}

	// Get the version of the Vulkan loader:
	printf(" ---> Getting Vulkan loader version.\n");
	uint32_t loader_version = VK_MAKE_API_VERSION(0, 1, 0, 0);
	if (vkEnumerateInstanceVersion)
	{
		if (vkEnumerateInstanceVersion(&loader_version) != VK_SUCCESS)
		{
			fprintf(stderr, "Error: Failed to get instance version!\n");
			destroy_vulkan_base(base, level);
			return -1;
		}
	}

	// Print out the version of the Vulkan loader:
	printf("      - Vulkan loader version: %d.%d.%d (variant %d).\n",
		VK_API_VERSION_MAJOR(loader_version),
		VK_API_VERSION_MINOR(loader_version),
		VK_API_VERSION_PATCH(loader_version),
		VK_API_VERSION_VARIANT(loader_version)
	);

	// Initialize GLFW and make sure Vulkan is supported:
	printf(" ---> Creating GLFW window and Vulkan surface.\n");

	// Create Vulkan instance:
	printf(" ---> Creating the Vulkan instance.\n");
	if (create_vulkan_instance(base, validation) != 0);
	{
		fprintf(stderr, "Error: Failed to create the Vulkan instance!\n");
		destroy_vulkan_base(base, level);
		return -1;
	}
	level++;

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	return 0;
}

// Destroy Vulkan environment based on data structure:
int destroy_vulkan_base(FracRenderVulkanBase *base, int level)
{
	// Level argument says how much of the data structure is populated.
	if (level > 1)
	{
		// Destroy Vulkan instance:
		vkDestroyInstance(base->vulkan_instance, NULL);
		base->vulkan_instance = VK_NULL_HANDLE;
	}
	if (level > 0)
	{
		// Destroy GLFW window and surface:
	}

	return 0;
}

// Create GLFW window and KHR surface:
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

	// Get supported

	return 0;
}

// Create the Vulkan instance:
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

	// Get validation layers:


	// Get GLFW extensions:
	uint32_t glfw_extension_count = 0;
	const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

	// Define instance creation info:
	VkInstanceCreateInfo instanceCreateInfo = {
		.sType				= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext				= NULL,
		.flags				= 0,
		.pApplicationInfo		= &applicationInfo,
		.enabledLayerCount		= 0,
		.ppEnabledLayerNames		= NULL,
		.enabledExtensionCount		= glfw_extension_count,
		.ppEnabledExtensionNames	= glfw_extensions
	};

	// Create the instance:
	base->vulkan_instance = VK_NULL_HANDLE;
	if (vkCreateInstance(&instanceCreateInfo, NULL, &base->vulkan_instance) != VK_SUCCESS)
	{
		base->vulkan_instance = VK_NULL_HANDLE;
		return -1;
	}

	return 0;
}
