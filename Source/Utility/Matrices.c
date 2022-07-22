#include "Matrices.h"

/************
 * Matrices *
 ************/

// Get value at [column, row]:
float get_element(FracRenderMatrix4 *matrix, uint32_t column, uint32_t row)
{
	return matrix->matrix[(4 * column) + row];
}

// Set value at [column, row]:
void set_element(FracRenderMatrix4 *matrix, uint32_t column, uint32_t row, float value)
{
	matrix->matrix[(4 * column) + row] = value;
}

// Get identity matrix:
FracRenderMatrix4 get_identity_matrix()
{
	FracRenderMatrix4 identity_matrix;
	for (uint32_t i = 0; i < 4; i++)
	{
		for (uint32_t j = 0; j < 4; j++)
		{
			if (i == j)
			{
				identity_matrix.matrix[(4 * i) + j] = 1.f;
				continue;
			}

			identity_matrix.matrix[(4 * i) + j] = 0.f;
		}
	}

	return identity_matrix;
}

// Print contents of matrix for debugging:
void print_matrix(FracRenderMatrix4 *matrix)
{
	// First row:
	printf("%.1f\t", get_element(matrix, 0, 0));
	printf("%.1f\t", get_element(matrix, 1, 0));
	printf("%.1f\t", get_element(matrix, 2, 0));
	printf("%.1f\n\n", get_element(matrix, 3, 0));

	// Second row:
	printf("%.1f\t", get_element(matrix, 0, 1));
	printf("%.1f\t", get_element(matrix, 1, 1));
	printf("%.1f\t", get_element(matrix, 2, 1));
	printf("%.1f\n\n", get_element(matrix, 3, 1));

	// Third row:
	printf("%.1f\t", get_element(matrix, 0, 2));
	printf("%.1f\t", get_element(matrix, 1, 2));
	printf("%.1f\t", get_element(matrix, 2, 2));
	printf("%.1f\n\n", get_element(matrix, 3, 2));

	// Fourth row:
	printf("%.1f\t", get_element(matrix, 0, 3));
	printf("%.1f\t", get_element(matrix, 1, 3));
	printf("%.1f\t", get_element(matrix, 2, 3));
	printf("%.1f\n\n", get_element(matrix, 3, 3));
}

// Get camera matrix from position, front and up:
FracRenderMatrix4 look_at(FracRenderVector3 *position, FracRenderVector3 *front,
							FracRenderVector3 *up)
{
	FracRenderMatrix4 result;

	FracRenderVector3 x;
	FracRenderVector3 y;
	FracRenderVector3 z;

	// Create new coordinate system:
	z.x = position->x - front->x;
	z.y = position->y - front->y;
	z.z = position->z - front->z;
	z = normalize(&z);
	y = *up;
	x = cross(&y, &z);

	y = cross(&z, &x);

	x = normalize(&x);
	y = normalize(&y);

	// Put values into the matrix:
	set_element(&result, 0, 0, x.x);
	set_element(&result, 1, 0, x.y);
	set_element(&result, 2, 0, x.z);
	set_element(&result, 3, 0, -dot(&x, position));
	set_element(&result, 0, 1, y.x);
	set_element(&result, 1, 1, y.y);
	set_element(&result, 2, 1, y.z);
	set_element(&result, 3, 1, -dot(&y, position));
	set_element(&result, 0, 2, z.x);
	set_element(&result, 1, 2, z.y);
	set_element(&result, 2, 2, z.z);
	set_element(&result, 3, 2, -dot(&z, position));
	set_element(&result, 0, 3, 0.f);
	set_element(&result, 1, 3, 0.f);
	set_element(&result, 2, 3, 0.f);
	set_element(&result, 3, 3, 1.f);

	return result;
}

/***********
 * Vectors *
 ***********/

// Get 4D vector with input values:
FracRenderVector4 initialize_vector_4(float x, float y, float z, float w)
{
	FracRenderVector4 vector;

	vector.x = x;
	vector.y = y;
	vector.z = z;
	vector.w = w;

	return vector;
}

// Multiply 4D vector by 4D matrix:
FracRenderVector4 matrix_by_vector_4(FracRenderMatrix4 *matrix, FracRenderVector4 *vector)
{
	FracRenderVector4 result;

	result.x =	(get_element(matrix, 0, 0) * vector->x) +
			(get_element(matrix, 1, 0) * vector->y) +
			(get_element(matrix, 2, 0) * vector->z) +
			(get_element(matrix, 3, 0) * vector->w);

	result.y =	(get_element(matrix, 0, 1) * vector->x) +
			(get_element(matrix, 1, 1) * vector->y) +
			(get_element(matrix, 2, 1) * vector->z) +
			(get_element(matrix, 3, 1) * vector->w);

	result.z =	(get_element(matrix, 0, 2) * vector->x) +
			(get_element(matrix, 1, 2) * vector->y) +
			(get_element(matrix, 2, 2) * vector->z) +
			(get_element(matrix, 3, 2) * vector->w);

	result.w =	(get_element(matrix, 0, 3) * vector->x) +
			(get_element(matrix, 1, 3) * vector->y) +
			(get_element(matrix, 2, 3) * vector->z) +
			(get_element(matrix, 3, 3) * vector->w);

	return result;
}

// Get 3D vector with input values:
FracRenderVector3 initialize_vector_3(float x, float y, float z)
{
	FracRenderVector3 vector;

	vector.x = x;
	vector.y = y;
	vector.z = z;

	return vector;
}

// Multiply 3D vector by 4D matrix:
FracRenderVector3 matrix_by_vector_3(FracRenderMatrix4 *matrix, FracRenderVector3 *vector)
{
	FracRenderVector4 vector_4 = initialize_vector_4(vector->x, vector->y, vector->z, 1.f);

	vector_4 = matrix_by_vector_4(matrix, &vector_4);

	return initialize_vector_3(vector_4.x, vector_4.y, vector_4.z);
}

// Get length of 3D vector:
float length(FracRenderVector3 *vector)
{
	if ((vector->x == 0.f) && (vector->y == 0.f) && (vector->z == 0.f)) { return 0.f; }

	return sqrt(
		(vector->x * vector->x) +
		(vector->y * vector->y) +
		(vector->z * vector->z)
	);
}

// Normalize 3D vector:
FracRenderVector3 normalize(FracRenderVector3 *vector)
{
	FracRenderVector3 result;
	result.x = vector->x;
	result.y = vector->y;
	result.z = vector->z;

	float vector_length = length(vector);

	if (vector_length != 0.f)
	{
		result.x /= vector_length;
		result.y /= vector_length;
		result.z /= vector_length;
	}

	return result;
}

// Cross product of 2 3D vectors:
FracRenderVector3 cross(FracRenderVector3 *vector_1, FracRenderVector3 *vector_2)
{
	FracRenderVector3 result;

	result.x =   (vector_1->y * vector_2->z) - (vector_1->z * vector_2->y);
	result.y = -((vector_1->x * vector_2->z) - (vector_1->z * vector_2->x));
	result.z =   (vector_1->x * vector_2->y) - (vector_1->y * vector_2->x);

	return result;
}

// Dot product of 2 3D vectors:
float dot(FracRenderVector3 *vector_1, FracRenderVector3 *vector_2)
{
	return	(vector_1->x * vector_2->x) +
		(vector_1->y * vector_2->y) +
		(vector_1->z * vector_2->z);
}

/**************************
 * Additional Mathematics *
 **************************/

// Convert degrees into radians:
float radians(float degrees)
{
	float pi = 3.14159f;

	return (degrees * pi) / 180.f;
}
