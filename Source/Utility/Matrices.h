#ifndef FRACRENDER_UTILITY_MATRICES_H
#define FRACRENDER_UTILITY_MATRICES_H

/***************************************************
 * Utilities for dealing with matrices and vectors *
 ***************************************************/

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

typedef struct {
	float x;
	float y;
	float z;
} FracRenderVector3;

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

// Get 4D vector with input values:
FracRenderVector4 initialize_vector_4(float x, float y, float z, float w);

// Multiply 4D vector by 4D matrix:
FracRenderVector4 matrix_by_vector_4(FracRenderMatrix4 *matrix, FracRenderVector4 *vector);

// Get 3D vector with input values:
FracRenderVector3 initialize_vector_3(float x, float y, float z);

// Multiply 3D vector by 4D matrix:
FracRenderVector3 matrix_by_vector_3(FracRenderMatrix4 *matrix, FracRenderVector3 *vector);

#endif
