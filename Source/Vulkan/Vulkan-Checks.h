#ifndef FRACRENDER_VULKAN_CHECKS_H
#define FRACRENDER_VULKAN_CHECKS_H

/*******************************************************
 * To check supported layers and extensions for Vulkan *
 *******************************************************/

// Library includes:
#include <stdio.h>

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

// Local includes:
#include "../../Third-Party/volk/include/volk/volk.h"
#include "Vulkan-Structs.h"

/***********************
 * Function Prototypes *
************************/

// Check support for Vulkan validation layers:
int check_layer_support();

// Check support for Vulkan extensions:
int check_extension_support();

#endif
