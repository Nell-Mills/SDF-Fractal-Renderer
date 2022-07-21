#ifndef FRACRENDER_UTILITY_MATRICES_H
#define FRACRENDER_UTILITY_MATRICES_H

// Library includes:
#include <stdio.h>
#include <stdint.h>

typedef struct {
	// Column major:
	float matrix[16];
} FracRenderMatrix4;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} FracRenderVector4;

/************
 * Matrices *
 ************/

// Get value at [column, row]:
float get_element(FracRenderMatrix4 *matrix, uint32_t column, uint32_t row);

// Set value at [column, row]:
void set_element(FracRenderMatrix4 *matrix, uint32_t column, uint32_t row, float value);

// Get identity matrix:
FracRenderMatrix4 get_identity_matrix();

// Print contents of matrix for debugging:
void print_matrix(FracRenderMatrix4 *matrix);

/***********
 * Vectors *
 ***********/

// Get vector with input values:
FracRenderVector4 initialize_vector(float x, float y, float z, float w);

// Multiply vector by matrix:
FracRenderVector4 matrix_by_vector(FracRenderMatrix4 *matrix, FracRenderVector4 *vector);

#endif
