#ifndef FRACRENDER_UTILITY_ANIMATION_H
#define FRACRENDER_UTILITY_ANIMATION_H

/******************************************************
 * Utilities for setting up automatic camera movement *
 ******************************************************/

// Library includes:
#include <stdio.h>

// Local includes:
#include "../Vulkan/01-Vulkan-Structs.h"

/***********************
 * Function Prototypes *
************************/

// Dummy function:
void update_animation_none(FracRenderProgramState *program_state);

// Fly through Hall of Pillars:
void update_animation_flythrough(FracRenderProgramState *program_state);

#endif
