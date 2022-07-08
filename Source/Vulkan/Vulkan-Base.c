#include "Vulkan-Base.h"

// Initialize Vulkan environment and populate data structure:
int initialize_vulkan_base(FracRenderVulkanBase *base,
	FracRenderVulkanLayersExtensions *layers_extensions)
{
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
	base->initialization_level++;

	// Create Vulkan instance:
	printf(" ---> Creating the Vulkan instance.\n");
	if (create_vulkan_instance(base, layers_extensions) != 0)
	{
		destroy_vulkan_base(base);
		return -1;
	}
	base->initialization_level++;

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	return 0;
}

// Destroy Vulkan environment based on data structure:
int destroy_vulkan_base(FracRenderVulkanBase *base)
{
	// Level argument says how much of the data structure is populated.
	if (base->initialization_level > 1)
	{
		// Destroy Vulkan instance:
		vkDestroyInstance(base->vulkan_instance, NULL);
		base->vulkan_instance = VK_NULL_HANDLE;
	}
	if (base->initialization_level > 0)
	{
		// Destroy GLFW window and surface:
		glfwTerminate();
	}

	base->initialization_level = 0;

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

	return 0;
}

// Create the Vulkan instance:
int create_vulkan_instance(FracRenderVulkanBase *base,
	FracRenderVulkanLayersExtensions *layers_extensions)
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

	// Gather all extension names:
	int max_name_length = 64;
	uint32_t extension_count = 0;
	extension_count += layers_extensions->num_validation_extensions;
	extension_count += layers_extensions->num_glfw_extensions;
	char **extensions = malloc(extension_count * sizeof(char *));

	int n = 0;
	for (int i = n; i < (n + layers_extensions->num_validation_extensions); i++)
	{
		extensions[i] = malloc(max_name_length * sizeof(char));
		strcpy(extensions[i], layers_extensions->validation_extensions[i - n]);
	}
	n += layers_extensions->num_validation_extensions;
	for (int i = n; i < (n + layers_extensions->num_glfw_extensions); i++)
	{
		extensions[i] = malloc(max_name_length * sizeof(char));
		strcpy(extensions[i], layers_extensions->glfw_extensions[i - n]);
	}

	// Define instance creation info:
	VkInstanceCreateInfo instanceCreateInfo = {
		.sType				= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext				= NULL,
		.flags				= 0,
		.pApplicationInfo		= &applicationInfo,
		.enabledLayerCount		= layers_extensions->num_validation_layers,
		.ppEnabledLayerNames		= (const char **)layers_extensions->
								validation_layers,
		.enabledExtensionCount		= extension_count,
		.ppEnabledExtensionNames	= (const char **)extensions
	};

	// Create the instance:
	base->vulkan_instance = VK_NULL_HANDLE;
	if (vkCreateInstance(&instanceCreateInfo, NULL, &base->vulkan_instance) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to create the Vulkan instance!\n");
		base->vulkan_instance = VK_NULL_HANDLE;
		return -1;
	}
	if (base->vulkan_instance == VK_NULL_HANDLE)
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
	volkLoadInstance(base->vulkan_instance);

	return 0;
}
