#include "02-Vulkan-Init.h"

// Initialize Volk and print some information:
int initialize_volk()
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Initializing Vulkan with Volk...\n");

	// Use Volk to load the Vulkan API:
	printf(" ---> Loading Volk.\n");
	if (volkInitialize() != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to initialize Volk!\n");
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

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
	return 0;
}

// Check support for Vulkan validation layers and extensions:
int check_validation_support(FracRenderVulkanValidation *validation)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Checking support for validation layers and extensions:\n");

	if (validation->num_validation_layers == 0) { return 0; }

	// Get number of instance layers:
	uint32_t num_layers = 0;
	if (vkEnumerateInstanceLayerProperties(&num_layers, NULL) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to enumerate instance layers!\n");
		return -1;
	}

	// Get instance layers:
	VkLayerProperties *layers = malloc(num_layers * sizeof(VkLayerProperties));
	if (vkEnumerateInstanceLayerProperties(&num_layers, layers) != VK_SUCCESS)
	{
		free(layers);
		fprintf(stderr, "Error: Unable to get instance layers!\n");
		return -1;
	}

	// Check for name match with required layers:
	for (uint32_t i = 0; i < validation->num_validation_layers; i++)
	{
		printf(" ---> %s...", validation->validation_layers[i]);

		int supported = -1;
		for (uint32_t j = 0; j < num_layers; j++)
		{
			if (strcmp(validation->validation_layers[i], layers[j].layerName) == 0)
			{
				supported = 0;
			}
		}
		if (supported != 0)
		{
			// Required layer not supported:
			free(layers);
			printf("\n");
			fprintf(stderr, "Error: Required layer \"%s\" not supported!\n",
							validation->validation_layers[i]);
			return -1;
		}
		printf(" Supported.\n");
	}

	free(layers);

	// Get number of instance extensions:
	uint32_t num_extensions = 0;
	if (vkEnumerateInstanceExtensionProperties(NULL, &num_extensions, NULL) != VK_SUCCESS)
	{
		fprintf(stderr, "Error: Unable to enumerate instance extensions!\n");
		return -1;
	}

	// Get instance extensions:
	VkExtensionProperties *extensions = malloc(num_extensions * sizeof(VkExtensionProperties));
	if (vkEnumerateInstanceExtensionProperties(NULL, &num_extensions,
						extensions) != VK_SUCCESS)
	{
		free(extensions);
		fprintf(stderr, "Error: Unable to get instance extensions!\n");
		return -1;
	}

	// Check for name match with required device extensions:
	for (uint32_t i = 0; i < validation->num_validation_extensions; i++)
	{
		printf(" ---> %s...", validation->validation_extensions[i]);

		int supported = -1;
		for (uint32_t j = 0; j < num_extensions; j++)
		{
			if (strcmp(validation->validation_extensions[i],
					extensions[j].extensionName) == 0)
			{
				supported = 0;
			}
		}
		if (supported != 0)
		{
			// Required extension not supported:
			free(extensions);
			printf("\n");
			fprintf(stderr, "Error: Required extension \"%s\" not supported!\n",
						validation->validation_extensions[i]);
			return -1;
		}
		printf(" Supported.\n");
	}

	free(extensions);

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
	return 0;
}
