#include "Animation.h"

/***************************************************************************************
 * Important reminder - animations should have a fixed step per FRAME, not per SECOND. *
 ***************************************************************************************/

// Dummy function:
void update_animation_none(FracRenderProgramState *program_state)
{
	// Do nothing.
	printf("Position: %f, %f, %f\n", program_state->position.x,
		program_state->position.y, program_state->position.z);
}

// Fly through Hall of Pillars:
void update_animation_flythrough(FracRenderProgramState *program_state)
{
	/* Animation runthrough:
	 * Frame 0:
	 */
}
