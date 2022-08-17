#ifndef FRACRENDER_PROGRAM_STATE_H
#define FRACRENDER_PROGRAM_STATE_H

/***********************************************************
 * Structure for keeping track of the global program state *
 ***********************************************************/

// Local includes:
#include "Vectors.h"

typedef struct {
	// Camera (eye):
	FracRenderVector3 position;
	FracRenderVector3 front;
	FracRenderVector3 up;

	// Rotation:
	double pitch;
	double yaw;

	// Time:
	double last_update;
	double current_update;
	double delta_t;

	// Frames:
	int frames;
	double frame_time;

	// Animation:
	uint64_t animation_frames;
	uint64_t max_animation_frames;

	// Controls:
	float base_movement_speed;
	float mouse_sensitivity;

	// Program settings:
	int fractal_type;
	int optimize;
	int animation;
	int performance;

	// Name of performance file:
	char performance_file_name[256];

	// Fractal parameter:
	float fractal_parameter;
	float fractal_parameter_start;
	float fractal_parameter_min;
	float fractal_parameter_max;
} FracRenderProgramState;

#endif
