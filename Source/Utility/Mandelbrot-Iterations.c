#include "Mandelbrot-Iterations.h"

void print_mandelbrot_2d_iterations(float c_x, float c_y, int for_shaders)
{
	float z_x = 0.f;
	float z_y = 0.f;

	int max_iterations = 25;
	float threshold = 2.f;

	printf("First 25 iterations of the Mandelbrot set for point (%f, %f):\n", c_x, c_y);

	for (int i = 0; i < max_iterations; i++)
	{
		// Z = Z^2 + C:
		z_x = (z_x * z_x) - (z_y * z_y) + c_x;
		z_y = (z_x * z_y * 2.f) + c_y;

		// Print result:
		if (for_shaders == 0) { printf("\t\tvec2("); }
		else
		{
			if (i > 998) { printf("Iteration %d: ", i + 1); }
			else if (i > 98) { printf("Iteration  %d: ", i + 1); }
			else if (i > 8) { printf("Iteration   %d: ", i + 1); }
			else { printf("Iteration    %d: ", i + 1); }
		}

		if (z_x < 0.f) { printf("%ff, ", z_x); }
		else { printf(" %ff, ", z_x); }

		if (z_y < 0.f) { printf("%ff", z_y); }
		else { printf(" %ff", z_y); }

		if (for_shaders == 0) { printf("),\n"); }
		else { printf("\n"); }

		// Check threshold:
		if (sqrt((z_x * z_x) + (z_y * z_y)) > threshold)
		{
			break;
		}
	}
}
