#include "Animation.h"

/***************************************************************************************
 * Important reminder - animations should have a fixed step per FRAME, not per SECOND. *
 * Also don't move the camera during an animation or you'll mess it up.                *
 ***************************************************************************************/

// Dummy function:
void update_animation_none(FracRenderProgramState *program_state)
{
	// Do nothing.
}

// Fly through Hall of Pillars:
void update_animation_flythrough(FracRenderProgramState *program_state)
{
	int num_key_frames = 23;

	uint64_t key_frames[] = {
		0,	//  0. Start.
		100,	//  1. Straight forward, tilt up.
		250,	//  2. Start turning left and moving left.
		250,	//  3. Finish turning left and moving left.
		300,	//  4. Move down into small tunnel.
		150,	//  5. Move through tunnel and tilt up.
		200,	//  6. Move into round room, tilt back to even and turn left.
		200,	//  7. Move straight forward.
		500,	//  8. Tilt up through the ceiling.
		500,	//  9. Continue upwards in huge pillar room.
		500,	// 10. Level out.
		300,	// 11. Dive into another tunnel.
		100,	// 12. Follow into yet another tunnel.
		100,	// 13. Smaller tunnel to the left. Yellow.
		100,	// 14. Prepare to leave tunnels.
		150,	// 15. Leave tunnels.
		200,	// 16. Level out.
		300,	// 17. Move forward. Green archway.
		200,	// 18. Turn right.
		750,	// 19. Move forward.
		150,	// 20. Go towards balls.
		150,	// 21. Go into more tunnels.
		250	// 22. Into round room.
	};

	uint64_t total_frames = 0;
	for (int i = 0; i < num_key_frames; i++) { total_frames += key_frames[i]; }
	if (program_state->animation_frames > total_frames) { return; }

	FracRenderVector3 key_positions[] = {
		initialize_vector_3(25.f, 20.f, 9.f),
		initialize_vector_3(25.f, 40.f, 90.f),
		initialize_vector_3(175.f, 40.f, 230.f),
		initialize_vector_3(225.f, 40.f, 435.f),
		initialize_vector_3(360.f, 60.f, 470.f),
		initialize_vector_3(410.f, 65.f, 475.f),
		initialize_vector_3(600.f, 50.f, 480.f),
		initialize_vector_3(775.f, 60.f, 375.f),
		initialize_vector_3(955.f, -145.f, 280.f),
		initialize_vector_3(1150.f, -565.f, 650.f),
		initialize_vector_3(1925.f, -1315.f, 2900.f),
		initialize_vector_3(555.f, -560.f, 4670.f),
		initialize_vector_3(-230.f, -400.f, 4835.f),
		initialize_vector_3(-500.f, -260.f, 5190.f),
		initialize_vector_3(-520.f, -235.f, 5370.f),
		initialize_vector_3(-810.f, -400.f, 5835.f),
		initialize_vector_3(-2415.f, -1875.f, 6865.f),
		initialize_vector_3(-5585.f, -1777.f, 6888.f),
		initialize_vector_3(-6780.f, -1777.f, 5165.f),
		initialize_vector_3(-6825.f, -1777.f, -10775.f),
		initialize_vector_3(-5945.f, -1565.f, -17690.f),
		initialize_vector_3(-6140.f, -1855.f, -27100.f),
		initialize_vector_3(-9900.f, -2050.f, -30970.f)
	};

	FracRenderVector3 key_fronts[] = {
		normalize(initialize_vector_3(0.f, 0.45f, 1.f)),
		normalize(initialize_vector_3(0.f, 0.f, 1.f)),
		normalize(initialize_vector_3(0.5f, 0.f, 0.65f)),
		normalize(initialize_vector_3(1.f, 0.f, 0.3f)),
		normalize(initialize_vector_3(0.9f, 0.4f, 0.1f)),
		normalize(initialize_vector_3(0.95f, -0.3f, 0.1f)),
		normalize(initialize_vector_3(0.85f, 0.f, -0.5f)),
		normalize(initialize_vector_3(0.85f, 0.f, -0.5f)),
		normalize(initialize_vector_3(0.35f, -0.9f, -0.25f)),
		normalize(initialize_vector_3(0.5f, -0.4f, 0.75f)),
		normalize(initialize_vector_3(-0.175f, 0.f, 0.98f)),
		normalize(initialize_vector_3(-0.85f, 0.55f, 0.075f)),
		normalize(initialize_vector_3(-0.75f, 0.25f, 0.6f)),
		normalize(initialize_vector_3(0.2f, 0.35f, 0.9f)),
		normalize(initialize_vector_3(-0.35f, 0.f, 0.95f)),
		normalize(initialize_vector_3(-0.6f, -0.5f, 0.6f)),
		normalize(initialize_vector_3(-1.f, 0.f, 0.f)),
		normalize(initialize_vector_3(-1.f, 0.f, 0.f)),
		normalize(initialize_vector_3(0.f, 0.f, -1.f)),
		normalize(initialize_vector_3(0.f, 0.f, -1.f)),
		normalize(initialize_vector_3(0.5f, 0.f, -0.85f)),
		normalize(initialize_vector_3(-0.125, 0.f, -1.f)),
		normalize(initialize_vector_3(-0.7f, 0.f, -0.7f))
	};

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
