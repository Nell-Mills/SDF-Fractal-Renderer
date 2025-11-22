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

	// P prints the current position and front:
	if ((key == GLFW_KEY_P) && (action == GLFW_PRESS))
	{
		// Get program state:
		FracRenderProgramState *program_state = (FracRenderProgramState *)
						glfwGetWindowUserPointer(window);
		printf("Position:\t%f\t%f\t%f\n", program_state->position.x,
			program_state->position.y, program_state->position.z);
		printf("Front   :\t%f\t%f\t%f\n\n", program_state->front.x,
			program_state->front.y, program_state->front.z);
	}
}

// GLFW mouse position callback:
void glfw_callback_mouse_position(GLFWwindow *window, double x_pos, double y_pos)
{
	static double last_x = 0.0;
	static double last_y = 0.0;

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
		program_state->pitch += dy * program_state->delta_t *
					program_state->mouse_sensitivity;
		if (program_state->pitch > 89.0) { program_state->pitch = 89.0; }
		if (program_state->pitch < -89.0) { program_state->pitch = -89.0; }

		// Update yaw:
		program_state->yaw += (dx * program_state->delta_t *
			program_state->mouse_sensitivity) / aspect;

		// Calculate rotation values:
		FracRenderVector3 camera_direction;
		camera_direction.x = (float)(cos(radians(program_state->yaw)) *
					cos(radians(program_state->pitch)));

		camera_direction.y = (float)(sin(radians(program_state->pitch)));

		camera_direction.z = (float)(sin(radians(program_state->yaw)) *
					cos(radians(program_state->pitch)));

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

	// Get speed multipliers:
	static float user_speed_multiplier = 1.f;
	float speed_multiplier_x_y = 1.f * user_speed_multiplier;
	float speed_multiplier_z = 1.f * user_speed_multiplier;
	if (program_state->fractal_type == -1)
	{
		// Move slower in x and y with more zoom:
		speed_multiplier_x_y /= fmax(1.f, program_state->position.z);

		// Move faster in z with more zoom:
		speed_multiplier_z *= fmax(1.f, program_state->position.z);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
	{
		user_speed_multiplier *= 1.05f;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
	{
		user_speed_multiplier /= 1.05f;
	}
	else if (glfwGetKey(window, GLFW_KEY_KP_MULTIPLY == GLFW_PRESS))
	{
		user_speed_multiplier = 1.f;
	}

	// Get screen aspect ratio:
	int screen_width;
	int screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);
	float aspect = (float)(screen_width) / (float)(screen_height);

	// Movement keys. Start with forward and back (forward takes priority):
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		program_state->position.x += (float)(program_state->base_movement_speed *
			speed_multiplier_z * program_state->delta_t) * program_state->front.x;
		program_state->position.y += (float)(program_state->base_movement_speed *
			speed_multiplier_z * program_state->delta_t) * program_state->front.y;
		program_state->position.z += (float)(program_state->base_movement_speed *
			speed_multiplier_z * program_state->delta_t) * program_state->front.z;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		program_state->position.x -= (float)(program_state->base_movement_speed *
			speed_multiplier_z * program_state->delta_t) * program_state->front.x;
		program_state->position.y -= (float)(program_state->base_movement_speed *
			speed_multiplier_z * program_state->delta_t) * program_state->front.y;
		program_state->position.z -= (float)(program_state->base_movement_speed *
			speed_multiplier_z * program_state->delta_t) * program_state->front.z;
	}

	// Left and right. Left takes priority:
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		FracRenderVector3 cross_f_u = cross(program_state->front, program_state->up);
		cross_f_u = normalize(cross_f_u);

		program_state->position.x -= (float)(program_state->base_movement_speed *
			speed_multiplier_x_y * program_state->delta_t) * cross_f_u.x / aspect;
		program_state->position.y -= (float)(program_state->base_movement_speed *
			speed_multiplier_x_y * program_state->delta_t) * cross_f_u.y / aspect;
		program_state->position.z -= (float)(program_state->base_movement_speed *
			speed_multiplier_x_y * program_state->delta_t) * cross_f_u.z / aspect;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		FracRenderVector3 cross_f_u = cross(program_state->front, program_state->up);
		cross_f_u = normalize(cross_f_u);

		program_state->position.x += (float)(program_state->base_movement_speed *
			speed_multiplier_x_y * program_state->delta_t) * cross_f_u.x / aspect;
		program_state->position.y += (float)(program_state->base_movement_speed *
			speed_multiplier_x_y * program_state->delta_t) * cross_f_u.y / aspect;
		program_state->position.z += (float)(program_state->base_movement_speed *
			speed_multiplier_x_y * program_state->delta_t) * cross_f_u.z / aspect;
	}

	// Up and down. Up takes priority:
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		program_state->position.x -= (float)(program_state->base_movement_speed *
			speed_multiplier_x_y * program_state->delta_t) * program_state->up.x;
		program_state->position.y -= (float)(program_state->base_movement_speed *
			speed_multiplier_x_y * program_state->delta_t) * program_state->up.y;
		program_state->position.z -= (float)(program_state->base_movement_speed *
			speed_multiplier_x_y * program_state->delta_t) * program_state->up.z;
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		program_state->position.x += (float)(program_state->base_movement_speed *
			speed_multiplier_x_y * program_state->delta_t) * program_state->up.x;
		program_state->position.y += (float)(program_state->base_movement_speed *
			speed_multiplier_x_y * program_state->delta_t) * program_state->up.y;
		program_state->position.z += (float)(program_state->base_movement_speed *
			speed_multiplier_x_y * program_state->delta_t) * program_state->up.z;
	}
}
