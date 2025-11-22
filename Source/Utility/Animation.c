#include "Animation.h"

/***************************************************************************************
 * Important reminder - animations should have a fixed step per FRAME, not per SECOND. *
 * Also don't move the camera during an animation or you'll mess it up.                *
 ***************************************************************************************/

// Dummy function:
void update_animation_none(FracRenderProgramState *program_state)
{
	// Do nothing.
	program_state->max_animation_frames = 0;
}

// Vary fractal parameter over time:
void update_animation_parameter(FracRenderProgramState *program_state)
{
	int num_key_frames = 4;

	// If Mandelbrot is being shown, slow down animation:
	uint64_t key_frames[4];

	if (program_state->fractal_type == -1)
	{
		key_frames[0] = 0;	//  0. Start.
		key_frames[1] = 9000;	//  1. Parameter increasing to top.
		key_frames[2] = 18000;	//  2. Parameter decreasing to bottom.
		key_frames[3] = 9000;	//  3. Parameter increasing to start.
	}
	else
	{
		key_frames[0] = 0;	//  0. Start.
		key_frames[1] = 2000;	//  1. Parameter increasing to top.
		key_frames[2] = 4000;	//  2. Parameter decreasing to bottom.
		key_frames[3] = 2000;	//  3. Parameter increasing to start.
	}

	// Get total frames:
	uint64_t total_frames = 0;
	for (int i = 0; i < num_key_frames; i++) { total_frames += key_frames[i]; }
	program_state->max_animation_frames = total_frames;
	if (program_state->animation_frames > total_frames) { return; }

	float key_parameter_values[] = {
		program_state->fractal_parameter_start,
		program_state->fractal_parameter_max,
		program_state->fractal_parameter_min,
		program_state->fractal_parameter_start
	};

	if (program_state->animation_frames == 0)
	{
		program_state->fractal_parameter = program_state->fractal_parameter_start;
		return;
	}

	// Update the animation:
	uint64_t frames = 0;
	for (int i = 1; i < num_key_frames; i++)
	{
		frames += key_frames[i];
		if (program_state->animation_frames < frames)
		{
			program_state->fractal_parameter += (key_parameter_values[i] -
				key_parameter_values[i - 1]) / (float)(key_frames[i]);

			break;
		}
		else if (program_state->animation_frames == key_frames[i])
		{
			// Set parameter manually to keep on track:
			program_state->fractal_parameter = key_parameter_values[i];
			break;
		}
	}
}

// Fly through Hall of Pillars:
void update_animation_flythrough(FracRenderProgramState *program_state)
{
	int num_key_frames = 25;

	uint64_t key_frames[] = {
		0,	//  0. Frame     0. Start.
		100,	//  1. Frame   100. Straight forward, tilt up.
		250,	//  2. Frame   350. Start turning left and moving left.
		250,	//  3. Frame   600. Finish turning left and moving left.
		300,	//  4. Frame   900. Move down into small tunnel.
		150,	//  5. Frame  1050. Move through tunnel and tilt up.
		200,	//  6. Frame  1250. Move into round room, tilt back to even and turn left.
		200,	//  7. Frame  1450. Move straight forward.
		500,	//  8. Frame  1950. Go swift.
		1000,	//  9. Frame  2950. Continue upwards in huge pillar room.
		2000,	// 10. Frame  4950. Level out.
		2000,	// 11. Frame  6950. Dive into another tunnel.
		100,	// 12. Frame  7050. Follow into yet another tunnel.
		100,	// 13. Frame  7150. Smaller tunnel to the left. Yellow.
		100,	// 14. Frame  7250. Prepare to leave tunnels.
		150,	// 15. Frame  7400. Leave tunnels.
		200,	// 16. Frame  7600. Level out.
		1000,	// 17. Frame  8600. Move forward. Green archway.
		200,	// 18. Frame  8800. Turn right.
		200,	// 19. Frame  9000. Turn right.
		1000,	// 20. Frame 10000. Move extra slowly back to orange archway.
		100,	// 21. Frame 10100. Start moving up into top of archway.
		100,	// 22. Frame 10200. "Aim towards that".
		100,	// 23. Frame 10300. "And that".
		1000	// 24. Frame 11300. Go small.
	};

	// Get total frames:
	uint64_t total_frames = 0;
	for (int i = 0; i < num_key_frames; i++) { total_frames += key_frames[i]; }
	program_state->max_animation_frames = total_frames;
	if (program_state->animation_frames > total_frames) { return; }

	FracRenderVector3 key_positions[] = {
		initialize_vector_3(25.f, 20.f, 9.f),		//  0.
		initialize_vector_3(25.f, 40.f, 90.f),		//  1.
		initialize_vector_3(175.f, 40.f, 230.f),	//  3.
		initialize_vector_3(225.f, 40.f, 435.f),	//  4.
		initialize_vector_3(360.f, 60.f, 470.f),	//  5.
		initialize_vector_3(410.f, 65.f, 475.f),	//  6.
		initialize_vector_3(600.f, 50.f, 480.f),	//  7.
		initialize_vector_3(775.f, 60.f, 375.f),	//  8.
		initialize_vector_3(955.f, -145.f, 280.f),	//  9.
		initialize_vector_3(1150.f, -565.f, 650.f),	// 10.
		initialize_vector_3(1925.f, -1315.f, 2900.f),	// 11.
		initialize_vector_3(555.f, -560.f, 4670.f),	// 12.
		initialize_vector_3(-230.f, -400.f, 4835.f),	// 13.
		initialize_vector_3(-500.f, -260.f, 5190.f),	// 14.
		initialize_vector_3(-520.f, -235.f, 5370.f),	// 15.
		initialize_vector_3(-810.f, -400.f, 5835.f),	// 16.
		initialize_vector_3(-2415.f, -1875.f, 6865.f),	// 17.
		initialize_vector_3(-5585.f, -1777.f, 6888.f),	// 18.
		initialize_vector_3(-6780.f, -1777.f, 5165.f),	// 19.
		initialize_vector_3(-6720.f, -1777.f, 4200.f),	// 20.
		initialize_vector_3(1715.f, -1777.f, 4200.f),	// 21.
		initialize_vector_3(1370.f, -2222.f, 4280.f),	// 21.
		initialize_vector_3(2000.f, -3120.f, 3400.f),	// 23.
		initialize_vector_3(2210.f, -3465.f, 2950.f),	// 24.
		initialize_vector_3(2490.f, -3625.f, 2700.f)	// 25.
	};

	FracRenderVector3 key_fronts[] = {
		normalize(initialize_vector_3(0.f, 0.45f, 1.f)),	//  0.
		normalize(initialize_vector_3(0.f, 0.f, 1.f)),		//  1.
		normalize(initialize_vector_3(0.5f, 0.f, 0.65f)),	//  2.
		normalize(initialize_vector_3(1.f, 0.f, 0.3f)),		//  3.
		normalize(initialize_vector_3(0.9f, 0.4f, 0.1f)),	//  4.
		normalize(initialize_vector_3(0.95f, -0.3f, 0.1f)),	//  5.
		normalize(initialize_vector_3(0.85f, 0.f, -0.5f)),	//  6.
		normalize(initialize_vector_3(0.85f, 0.f, -0.5f)),	//  7.
		normalize(initialize_vector_3(0.35f, -0.9f, -0.25f)),	//  8.
		normalize(initialize_vector_3(0.5f, -0.4f, 0.75f)),	//  9.
		normalize(initialize_vector_3(-0.175f, 0.f, 0.98f)),	// 10.
		normalize(initialize_vector_3(-0.85f, 0.55f, 0.075f)),	// 11.
		normalize(initialize_vector_3(-0.75f, 0.25f, 0.6f)),	// 12.
		normalize(initialize_vector_3(0.2f, 0.35f, 0.9f)),	// 13.
		normalize(initialize_vector_3(-0.35f, 0.f, 0.95f)),	// 14.
		normalize(initialize_vector_3(-0.6f, -0.5f, 0.6f)),	// 15.
		normalize(initialize_vector_3(-1.f, 0.f, 0.f)),		// 16.
		normalize(initialize_vector_3(-1.f, 0.f, 0.f)),		// 17.
		normalize(initialize_vector_3(0.f, 0.f, -1.f)),		// 18.
		normalize(initialize_vector_3(1.f, 0.f, 0.f)),		// 19.
		normalize(initialize_vector_3(1.f, 0.f, 0.f)),		// 20.
		normalize(initialize_vector_3(0.5f, -0.75f, -0.5f)),	// 21.
		normalize(initialize_vector_3(0.45f, -0.6f, -0.65f)),	// 22.
		normalize(initialize_vector_3(0.7f, -0.35f, -0.65f)),	// 23.
		normalize(initialize_vector_3(0.9f, -0.15f, 0.4f))	// 24.
	};

	if (program_state->animation_frames == 0)
	{
		program_state->position = key_positions[0];
		program_state->front = key_fronts[0];
		return;
	}

	// Update the animation:
	uint64_t frames = 0;
	for (int i = 1; i < num_key_frames; i++)
	{
		frames += key_frames[i];
		if (program_state->animation_frames < frames)
		{
			program_state->position.x += (key_positions[i].x - key_positions[i - 1].x)
									/ (float)(key_frames[i]);
			program_state->position.y += (key_positions[i].y - key_positions[i - 1].y)
									/ (float)(key_frames[i]);
			program_state->position.z += (key_positions[i].z - key_positions[i - 1].z)
									/ (float)(key_frames[i]);

			program_state->front.x += (key_fronts[i].x - key_fronts[i - 1].x)
								/ (float)(key_frames[i]);
			program_state->front.y += (key_fronts[i].y - key_fronts[i - 1].y)
								/ (float)(key_frames[i]);
			program_state->front.z += (key_fronts[i].z - key_fronts[i - 1].z)
								/ (float)(key_frames[i]);

			break;
		}
		else if (program_state->animation_frames == key_frames[i])
		{
			// Set position and front manually to keep on track:
			program_state->position = key_positions[i];
			program_state->front = key_fronts[i];
			break;
		}
	}
}

// Slowly slide to show artefacts:
void update_animation_artefacts(FracRenderProgramState *program_state)
{
	int num_key_frames = 2;

	uint64_t key_frames[] = {
		0,	//  0. Start.
		1000	//  1. Slide left.
	};

	uint64_t total_frames = 0;
	for (int i = 0; i < num_key_frames; i++) { total_frames += key_frames[i]; }
	program_state->max_animation_frames = total_frames;
	if (program_state->animation_frames > total_frames) { return; }

	FracRenderVector3 key_positions[] = {
		initialize_vector_3(560.f, -1885.f, 4180.f),
		initialize_vector_3(560.f, -1885.f, 1530.f)
	};

	FracRenderVector3 key_fronts[] = {
		normalize(initialize_vector_3(1.f, 0.f, 0.f)),
		normalize(initialize_vector_3(1.f, 0.f, 0.f))
	};

	if (program_state->animation_frames == 0)
	{
		program_state->position = key_positions[0];
		program_state->front = key_fronts[0];
		return;
	}

	// Update the animation:
	uint64_t frames = 0;
	for (int i = 1; i < num_key_frames; i++)
	{
		frames += key_frames[i];
		if (program_state->animation_frames < frames)
		{
			program_state->position.x += (key_positions[i].x - key_positions[i - 1].x)
									/ (float)(key_frames[i]);
			program_state->position.y += (key_positions[i].y - key_positions[i - 1].y)
									/ (float)(key_frames[i]);
			program_state->position.z += (key_positions[i].z - key_positions[i - 1].z)
									/ (float)(key_frames[i]);

			program_state->front.x += (key_fronts[i].x - key_fronts[i - 1].x)
								/ (float)(key_frames[i]);
			program_state->front.y += (key_fronts[i].y - key_fronts[i - 1].y)
								/ (float)(key_frames[i]);
			program_state->front.z += (key_fronts[i].z - key_fronts[i - 1].z)
								/ (float)(key_frames[i]);

			break;
		}
		else if (program_state->animation_frames == key_frames[i])
		{
			// Set position and front manually to keep on track:
			program_state->position = key_positions[i];
			program_state->front = key_fronts[i];
			break;
		}
	}
}
