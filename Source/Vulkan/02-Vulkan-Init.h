#ifndef FRACRENDER_VULKAN_INIT_H
#define FRACRENDER_VULKAN_INIT_H

/******************************************************************
 * To initialize Volk and check support for layers and extensions *
 ******************************************************************/

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

// Initialize Volk and print some information:
int initialize_volk();

// Check support for Vulkan validation layers and extensions:
int check_validation_support(FracRenderVulkanValidation *validation);

#endif
