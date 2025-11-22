#ifndef FRACRENDER_UTILITY_VECTORS_H
#define FRACRENDER_UTILITY_VECTORS_H

/*************************************************
 * Utilities for dealing with vectors and angles *
 *************************************************/

// Library includes:
#include <math.h>

typedef struct {
	float x;
	float y;
	float z;
} FracRenderVector3;

// Convert degrees into radians:
float radians(float degrees);

// Convert radians into degrees:
float degrees(float radians);

// Get axes in coordinate system of input:
void get_axes(FracRenderVector3 position, FracRenderVector3 forward, FracRenderVector3 up,
				FracRenderVector3 *x_axis, FracRenderVector3 *y_axis);

// Get 3D vector with input values:
FracRenderVector3 initialize_vector_3(float x, float y, float z);

// Get 3D vector from another 3D vector:
FracRenderVector3 clone_vector_3(FracRenderVector3 other_vector);

// Get length of 3D vector:
float length(FracRenderVector3 vector);

// Normalize 3D vector:
FracRenderVector3 normalize(FracRenderVector3 vector);

// Cross product of 2 3D vectors:
FracRenderVector3 cross(FracRenderVector3 vector_1, FracRenderVector3 vector_2);

// Dot product of 2 3D vectors:
float dot(FracRenderVector3 vector_1, FracRenderVector3 vector_2);

// Multiply vector by scalar:
FracRenderVector3 multiply_vector_3_scalar(FracRenderVector3 vector, float scalar);

// Add one vector to another:
FracRenderVector3 add_vector_3(FracRenderVector3 vector_1, FracRenderVector3 vector_2);

// Subtract one vector from another:
FracRenderVector3 subtract_vector_3(FracRenderVector3 vector_1, FracRenderVector3 vector_2);

// Clamp a vector to certain values:
FracRenderVector3 clamp_vector_3(FracRenderVector3 vector, FracRenderVector3 clamp_min,
							FracRenderVector3 clamp_max);

#endif
