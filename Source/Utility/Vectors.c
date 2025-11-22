#include "Vectors.h"

// Convert degrees into radians:
float radians(float degrees)
{
	float pi = 3.14159f;

	return (degrees * pi) / 180.f;
}

// Convert radians into degrees:
float degrees(float radians)
{
	float pi = 3.14159f;

	return (radians * 180.f) / pi;
}

// Get axes in coordinate system of input:
void get_axes(FracRenderVector3 position, FracRenderVector3 forward, FracRenderVector3 up,
				FracRenderVector3 *x_axis, FracRenderVector3 *y_axis)
{
	*x_axis = normalize(cross(forward, up));
	*y_axis = normalize(cross(*x_axis, forward));
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

// Get 3D vector from another 3D vector:
FracRenderVector3 clone_vector_3(FracRenderVector3 other_vector)
{
	FracRenderVector3 result;
	result.x = other_vector.x;
	result.y = other_vector.y;
	result.z = other_vector.z;

	return result;
}

// Get length of 3D vector:
float length(FracRenderVector3 vector)
{
	if ((vector.x == 0.f) && (vector.y == 0.f) && (vector.z == 0.f)) { return 0.f; }

	return sqrt(
		(vector.x * vector.x) +
		(vector.y * vector.y) +
		(vector.z * vector.z)
	);
}

// Normalize 3D vector:
FracRenderVector3 normalize(FracRenderVector3 vector)
{
	FracRenderVector3 result;
	result.x = vector.x;
	result.y = vector.y;
	result.z = vector.z;

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
FracRenderVector3 cross(FracRenderVector3 vector_1, FracRenderVector3 vector_2)
{
	FracRenderVector3 result;

	result.x =   (vector_1.y * vector_2.z) - (vector_1.z * vector_2.y);
	result.y = -((vector_1.x * vector_2.z) - (vector_1.z * vector_2.x));
	result.z =   (vector_1.x * vector_2.y) - (vector_1.y * vector_2.x);

	return result;
}

// Dot product of 2 3D vectors:
float dot(FracRenderVector3 vector_1, FracRenderVector3 vector_2)
{
	return	(vector_1.x * vector_2.x) +
		(vector_1.y * vector_2.y) +
		(vector_1.z * vector_2.z);
}

// Multiply vector by scalar:
FracRenderVector3 multiply_vector_3_scalar(FracRenderVector3 vector, float scalar)
{
	FracRenderVector3 result;
	result.x = vector.x * scalar;
	result.y = vector.y * scalar;
	result.z = vector.z * scalar;

	return result;
}

// Add one vector to another:
FracRenderVector3 add_vector_3(FracRenderVector3 vector_1, FracRenderVector3 vector_2)
{
	FracRenderVector3 result;
	result.x = vector_1.x + vector_2.x;
	result.y = vector_1.y + vector_2.y;
	result.z = vector_1.z + vector_2.z;

	return result;
}

// Subtract one vector from another:
FracRenderVector3 subtract_vector_3(FracRenderVector3 vector_1, FracRenderVector3 vector_2)
{
	FracRenderVector3 result;
	result.x = vector_1.x - vector_2.x;
	result.y = vector_1.y - vector_2.y;
	result.z = vector_1.z - vector_2.z;

	return result;
}

// Clamp a vector to certain values:
FracRenderVector3 clamp_vector_3(FracRenderVector3 vector, FracRenderVector3 clamp_min,
							FracRenderVector3 clamp_max)
{
	FracRenderVector3 result = vector;

	// Clamp x:
	if (result.x < clamp_min.x) { result.x = clamp_min.x; }
	else if (result.x > clamp_max.x) { result.x = clamp_max.x; }

	// Clamp y:
	if (result.y < clamp_min.y) { result.y = clamp_min.y; }
	else if (result.y > clamp_max.y) { result.y = clamp_max.y; }

	// Clamp z:
	if (result.z < clamp_min.z) { result.z = clamp_min.z; }
	else if (result.z > clamp_max.z) { result.z = clamp_max.z; }

	return result;
}
