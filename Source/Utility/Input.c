#include "Input.h"

// GLFW keyboard callback:
void glfw_callback_key_press(GLFWwindow *window, int key, int scan_code,
					int action, int modifier_flags)
{
	// Escape closes the window:
	if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

// GLFW mouse position callback:
void glfw_callback_mouse_position(GLFWwindow *window, double x_pos, double y_pos)
{

}

// Movement key polling function:
void poll_movement_keys(GLFWwindow *window)
{

}
