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
