#ifndef FRACRENDER_UTILITY_MATRICES_H
#define FRACRENDER_UTILITY_MATRICES_H

/***********************************************************
 * Utilities for dealing with matrices, vectors and angles *
 ***********************************************************/

// Library includes:
#include <stdio.h>
#include <stdint.h>
#include <math.h>

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
float get_element(FracRenderMatrix4 matrix, uint32_t column, uint32_t row);

// Set value at [column, row]:
void set_element(FracRenderMatrix4 *matrix, uint32_t column, uint32_t row, float value);

// Get identity matrix:
FracRenderMatrix4 get_identity_matrix();

// Print contents of matrix for debugging:
void print_matrix(FracRenderMatrix4 matrix);

// Get camera matrix from position, front and up:
FracRenderMatrix4 look_at(FracRenderVector3 position, FracRenderVector3 front,
							FracRenderVector3 up);

/***********
 * Vectors *
 ***********/

// Get 4D vector with input values:
FracRenderVector4 initialize_vector_4(float x, float y, float z, float w);

// Multiply 4D vector by 4D matrix:
FracRenderVector4 matrix_by_vector_4(FracRenderMatrix4 matrix, FracRenderVector4 vector);

// Get 3D vector with input values:
FracRenderVector3 initialize_vector_3(float x, float y, float z);

// Multiply 3D vector by 4D matrix:
FracRenderVector3 matrix_by_vector_3(FracRenderMatrix4 matrix, FracRenderVector3 vector);

// Get length of 3D vector:
float length(FracRenderVector3 vector);

// Normalize 3D vector:
FracRenderVector3 normalize(FracRenderVector3 vector);

// Cross product of 2 3D vectors:
FracRenderVector3 cross(FracRenderVector3 vector_1, FracRenderVector3 vector_2);

// Dot product of 2 3D vectors:
float dot(FracRenderVector3 vector_1, FracRenderVector3 vector_2);

// Get axes in coordinate system of input:
void get_axes(FracRenderVector3 position, FracRenderVector3 forward, FracRenderVector3 up,
				FracRenderVector3 *x_axis, FracRenderVector3 *y_axis);

/**************************
 * Additional Mathematics *
 **************************/

// Convert degrees into radians:
float radians(float degrees);

#endif
