#ifndef FRACRENDER_UTILITY_ANIMATION_H
#define FRACRENDER_UTILITY_ANIMATION_H

/******************************************************
 * Utilities for setting up automatic camera movement *
 ******************************************************/

// Library includes:
#include <stdio.h>
#include <stdint.h>

// Local includes:
#include "Program-State.h"

/***********************
 * Function Prototypes *
************************/

// Dummy function:
void update_animation_none(FracRenderProgramState *program_state);

// Vary fractal parameter over time:
void update_animation_parameter(FracRenderProgramState *program_state);

// Fly through Hall of Pillars:
void update_animation_flythrough(FracRenderProgramState *program_state);

// Slowly slide to show artefacts:
void update_animation_artefacts(FracRenderProgramState *program_state);

#endif
