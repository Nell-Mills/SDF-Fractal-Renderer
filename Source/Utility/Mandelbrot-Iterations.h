#ifndef FRACRENDER_UTILITY_MANDELBROT_ITERATIONS_H
#define FRACRENDER_UTILITY_MANDELBROT_ITERATIONS_H

/*******************************************************************************************
 * This is for printing results of a certain number of iterations of the 2D Mandelbrot set *
 *******************************************************************************************/

// Library includes:
#include <stdio.h>
#include <math.h>

/***********************
 * Function Prototypes *
 ***********************/

void print_mandelbrot_2d_iterations(float c_x, float c_y, int for_shaders);

#endif
