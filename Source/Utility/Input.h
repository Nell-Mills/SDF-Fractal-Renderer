#ifndef FRACRENDER_UTILITY_INPUT_H
#define FRACRENDER_UTILITY_INPUT_H

/**************************************************************
 * Utilities for dealing with user input (keyboard and mouse) *
 **************************************************************/

// Library includes:
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

// Local includes:
#include "Program-State.h"

/***********************
 * Function Prototypes *
************************/

// GLFW keyboard callback:
void glfw_callback_key_press(GLFWwindow *window, int key, int scan_code,
					int action, int modifier_flags);

// GLFW mouse position callback:
void glfw_callback_mouse_position(GLFWwindow *window, double x_pos, double y_pos);

// Movement key polling function:
void poll_movement_keys(GLFWwindow *window);

#endif
