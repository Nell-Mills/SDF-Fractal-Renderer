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

	// Tab toggles the cursor:
	if ((key == GLFW_KEY_TAB) && (action == GLFW_PRESS))
	{
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}

// GLFW mouse position callback:
void glfw_callback_mouse_position(GLFWwindow *window, double x_pos, double y_pos)
{
	static double last_x = 0.0;
	static double last_y = 0.0;

	static double pitch = 20.5;
	static double yaw = 89.0;

	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
	{
		// Get program state:
		FracRenderProgramState *program_state = (FracRenderProgramState *)
						glfwGetWindowUserPointer(window);

		// Get screen aspect ratio:
		int screen_width;
		int screen_height;
		glfwGetFramebufferSize(window, &screen_width, &screen_height);
		float aspect = (float)(screen_width) / (float)(screen_height);

		// Change in mouse position since last call:
		double dx = x_pos - last_x;
		double dy = y_pos - last_y;

		// Update pitch and clamp:
		pitch += dy * program_state->delta_t * program_state->mouse_sensitivity;
		if (pitch > 89.0) { pitch = 89.0; }
		if (pitch < -89.0) { pitch = -89.0; }

		// Update yaw:
		yaw += (dx * program_state->delta_t * program_state->mouse_sensitivity) / aspect;

		// Calculate rotation values:
		FracRenderVector3 camera_direction;
		camera_direction.x = (float)(cos(radians(yaw)) * cos(radians(pitch)));

		camera_direction.y = (float)(sin(radians(pitch)));

		camera_direction.z = (float)(sin(radians(yaw)) * cos(radians(pitch)));

		program_state->front = normalize(camera_direction);
	}

	last_x = x_pos;
	last_y = y_pos;
}

// Movement key polling function:
void poll_movement_keys(GLFWwindow *window)
{
	/* There is a separate polling function so that it runs every
	 * frame, to avoid the delay between key press and repeat. */

	// Get program state:
	FracRenderProgramState *program_state = (FracRenderProgramState *)
					glfwGetWindowUserPointer(window);

	// Get screen aspect ratio:
	int screen_width;
	int screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);
	float aspect = (float)(screen_width) / (float)(screen_height);

	// Movement keys. Start with forward and back (forward takes priority):
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))
	{
		program_state->position.x += (float)(program_state->base_movement_speed *
					program_state->delta_t) * program_state->front.x;
		program_state->position.y += (float)(program_state->base_movement_speed *
					program_state->delta_t) * program_state->front.y;
		program_state->position.z += (float)(program_state->base_movement_speed *
					program_state->delta_t) * program_state->front.z;
	}
	else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))
	{
		program_state->position.x -= (float)(program_state->base_movement_speed *
					program_state->delta_t) * program_state->front.x;
		program_state->position.y -= (float)(program_state->base_movement_speed *
					program_state->delta_t) * program_state->front.y;
		program_state->position.z -= (float)(program_state->base_movement_speed *
					program_state->delta_t) * program_state->front.z;
	}

	// Left and right. Left takes priority:
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A))
	{
		FracRenderVector3 cross_f_u = cross(program_state->front, program_state->up);
		cross_f_u = normalize(cross_f_u);

		program_state->position.x -= (float)(program_state->base_movement_speed *
					program_state->delta_t) * cross_f_u.x / aspect;
		program_state->position.y -= (float)(program_state->base_movement_speed *
					program_state->delta_t) * cross_f_u.y / aspect;
		program_state->position.z -= (float)(program_state->base_movement_speed *
					program_state->delta_t) * cross_f_u.z / aspect;
	}
	else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D))
	{
		FracRenderVector3 cross_f_u = cross(program_state->front, program_state->up);
		cross_f_u = normalize(cross_f_u);

		program_state->position.x += (float)(program_state->base_movement_speed *
					program_state->delta_t) * cross_f_u.x / aspect;
		program_state->position.y += (float)(program_state->base_movement_speed *
					program_state->delta_t) * cross_f_u.y / aspect;
		program_state->position.z += (float)(program_state->base_movement_speed *
					program_state->delta_t) * cross_f_u.z / aspect;
	}

	// Up and down. Up takes priority:
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_E))
	{
		program_state->position.x -= (float)(program_state->base_movement_speed *
					program_state->delta_t) * program_state->up.x;
		program_state->position.y -= (float)(program_state->base_movement_speed *
					program_state->delta_t) * program_state->up.y;
		program_state->position.z -= (float)(program_state->base_movement_speed *
					program_state->delta_t) * program_state->up.z;
	}
	else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Q))
	{
		program_state->position.x += (float)(program_state->base_movement_speed *
					program_state->delta_t) * program_state->up.x;
		program_state->position.y += (float)(program_state->base_movement_speed *
					program_state->delta_t) * program_state->up.y;
		program_state->position.z += (float)(program_state->base_movement_speed *
					program_state->delta_t) * program_state->up.z;
	}
}
