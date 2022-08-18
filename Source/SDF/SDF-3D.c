#include "SDF-3D.h"

// Set up 3D SDF structure:
void set_up_sdf_3d(FracRenderProgramState *program_state, FracRenderSDF3D *sdf_3d)
{
	// If 3D SDF is not being used, stop:
	if (program_state->optimize != 0) { return; }

	// Set up SDF info:
	if (program_state->fractal_type == 0)
	{
		sdf_3d->levels		= 8;
		sdf_3d->num_voxels	= pow(8, sdf_3d->levels);
		sdf_3d->size		= 1.2f;

		sdf_3d->centre		= initialize_vector_3(0.f, 0.f, 0.f);
	}
	else
	{
		sdf_3d->levels		= 9;
		sdf_3d->num_voxels	= pow(8, sdf_3d->levels);
		sdf_3d->size		= 500.f;

		sdf_3d->centre = program_state->position;
	}

	sdf_3d->fractal_type	= program_state->fractal_type;
	sdf_3d->voxels		= NULL;

	// Create 3D SDF:
	if (create_sdf_3d(sdf_3d) != 0)
	{
		destroy_sdf_3d(sdf_3d);
	}
}

// Calculate 3D SDF:
int create_sdf_3d(FracRenderSDF3D *sdf_3d)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Creating 3D SDF...\n");

	// Allocate memory for the SDF:
	printf(" ---> Allocating memory.\n");
	size_t memory_required = sdf_3d->num_voxels * sizeof(float);

	if ((memory_required / (1024 * 1024)) > 2000)
	{
		fprintf(stderr, "Error: Tried to allocate more than 2GB!\n");
		return -1;
	}

	sdf_3d->voxels = malloc(memory_required);
	printf("      - Memory allocated: %lu bytes (%lu MB).\n", memory_required,
						memory_required / (1024 * 1024));

	printf(" ---> Calculating distance values.\n");
	uint32_t current_index = 0;
	if (create_sdf_3d_helper(sdf_3d, sdf_3d->size, sdf_3d->centre, &current_index, 0) != 0)
	{
		return -1;
	}

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	return 0;
}

// SDF recursion helper:
int create_sdf_3d_helper(FracRenderSDF3D *sdf_3d, float size, FracRenderVector3 centre,
						uint32_t *current_index, uint32_t level)
{
	// Check index:
	if (*current_index >= sdf_3d->num_voxels)
	{
		fprintf(stderr, "Error: 3D SDF tried to exceed memory allocation!\n");
		return -1;
	}

	if (level == sdf_3d->levels)
	{
		// Max resolution. Calculate signed distance from surface:
		if (sdf_3d->fractal_type == 1)
		{
			// Hall of Pillars:
			float distance_estimate = signed_distance_function_hall_of_pillars(centre);

			// To guarantee underestimate, take away half length of diagonal of cube:
			distance_estimate -= (distance_estimate / fabs(distance_estimate)) *
									sqrt(3.f) * size;
			sdf_3d->voxels[*current_index] = distance_estimate;
		}
		else
		{
			// Default to Mandelbulb:
			float distance_estimate = signed_distance_function_mandelbulb(centre);

			// To guarantee underestimate, take away half length of diagonal of cube:
			distance_estimate -= (distance_estimate / fabs(distance_estimate)) *
									sqrt(3.f) * size;
			sdf_3d->voxels[*current_index] = distance_estimate;
		}
		*current_index += 1;
	}
	else
	{
		// Split cube into 8 smaller cubes:
		FracRenderVector3 new_centre;

		// Print progress:
		if (level == 0)
		{
			printf("      --->   0.0%%.\n");
		}

		// Upper, top-left cube (looking top-down):
		new_centre.x = centre.x - (size / 2.f);
		new_centre.y = centre.y + (size / 2.f);
		new_centre.z = centre.z + (size / 2.f);
		if (create_sdf_3d_helper(sdf_3d, size / 2.f, new_centre,
					current_index, level + 1) != 0)
		{
			return -1;
		}

		// Print progress:
		if (level == 0)
		{
			printf("      --->  12.5%%.\n");
		}

		// Upper, top-right cube:
		new_centre.x = centre.x + (size / 2.f);
		new_centre.y = centre.y + (size / 2.f);
		new_centre.z = centre.z + (size / 2.f);
		if (create_sdf_3d_helper(sdf_3d, size / 2.f, new_centre,
					current_index, level + 1) != 0)
		{
			return -1;
		}

		// Print progress:
		if (level == 0)
		{
			printf("      --->  25.0%%.\n");
		}

		// Upper, bottom-left cube:
		new_centre.x = centre.x - (size / 2.f);
		new_centre.y = centre.y + (size / 2.f);
		new_centre.z = centre.z - (size / 2.f);
		if (create_sdf_3d_helper(sdf_3d, size / 2.f, new_centre,
					current_index, level + 1) != 0)
		{
			return -1;
		}

		// Print progress:
		if (level == 0)
		{
			printf("      --->  37.5%%.\n");
		}

		// Upper, bottom-right cube:
		new_centre.x = centre.x + (size / 2.f);
		new_centre.y = centre.y + (size / 2.f);
		new_centre.z = centre.z - (size / 2.f);
		if (create_sdf_3d_helper(sdf_3d, size / 2.f, new_centre,
					current_index, level + 1) != 0)
		{
			return -1;
		}

		// Print progress:
		if (level == 0)
		{
			printf("      --->  50.0%%.\n");
		}

		// Lower, top-left cube (looking top-down):
		new_centre.x = centre.x - (size / 2.f);
		new_centre.y = centre.y - (size / 2.f);
		new_centre.z = centre.z + (size / 2.f);
		if (create_sdf_3d_helper(sdf_3d, size / 2.f, new_centre,
					current_index, level + 1) != 0)
		{
			return -1;
		}

		// Print progress:
		if (level == 0)
		{
			printf("      --->  62.5%%.\n");
		}

		// Lower, top-right cube:
		new_centre.x = centre.x + (size / 2.f);
		new_centre.y = centre.y - (size / 2.f);
		new_centre.z = centre.z + (size / 2.f);
		if (create_sdf_3d_helper(sdf_3d, size / 2.f, new_centre,
					current_index, level + 1) != 0)
		{
			return -1;
		}

		// Print progress:
		if (level == 0)
		{
			printf("      --->  75.0%%.\n");
		}

		// Lower, bottom-left cube:
		new_centre.x = centre.x - (size / 2.f);
		new_centre.y = centre.y - (size / 2.f);
		new_centre.z = centre.z - (size / 2.f);
		if (create_sdf_3d_helper(sdf_3d, size / 2.f, new_centre,
					current_index, level + 1) != 0)
		{
			return -1;
		}

		// Print progress:
		if (level == 0)
		{
			printf("      --->  87.5%%.\n");
		}

		// Lower, bottom-right cube:
		new_centre.x = centre.x + (size / 2.f);
		new_centre.y = centre.y - (size / 2.f);
		new_centre.z = centre.z - (size / 2.f);
		if (create_sdf_3d_helper(sdf_3d, size / 2.f, new_centre,
					current_index, level + 1) != 0)
		{
			return -1;
		}

		// Print progress:
		if (level == 0)
		{
			printf("      ---> 100.0%%.\n");
		}
	}

	return 0;
}

// Free SDF memory:
void destroy_sdf_3d(FracRenderSDF3D *sdf_3d)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Freeing memory for 3D SDF (CPU-side)...\n");

	if (sdf_3d->voxels)
	{
		free(sdf_3d->voxels);
	}

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
}

// Signed distance function for Mandelbulb fractal:
float signed_distance_function_mandelbulb(FracRenderVector3 position)
{
	int max_iterations = 4;
	float escape_radius = 2.f;
	float parameter = 8.f;

	FracRenderVector3 z = position;
	float dr = 1.f;
	float r = 0.f;

	for (int i = 0; i < max_iterations; i++)
	{
		r = length(z);
		if (r > escape_radius) { break; }

		// Convert position to spherical coordinates:
		float theta = acos(z.z / r);
		float phi = atan2(z.y, z.x);
		dr = (pow(r, parameter - 1.f) * parameter * dr) + 1.f;

		// Scale and rotate position:
		float zr = pow(r, parameter);
		theta *= parameter;
		phi *= parameter;

		// Convert position back to Cartesian coordinates:
		z.x = (zr * sin(theta) * cos(phi)) + position.x;
		z.y = (zr * sin(phi) * sin(theta)) + position.y;
		z.z = (zr * cos(theta)) + position.z;
	}

	return 0.5f * log(r) * (r / dr);
}

// Signed distance function for Hall of Pillars fractal:
float signed_distance_function_hall_of_pillars(FracRenderVector3 position)
{
	FracRenderVector3 z;
	z.x = position.x;
	z.y = position.z;
	z.z = position.y;
	float scale = 1.f;
	FracRenderVector3 size_clamp_min = initialize_vector_3(-1.f, -1.f, -1.3f);
	FracRenderVector3 size_clamp_max = initialize_vector_3(1.f, 1.f, 1.3f);

	for (int i = 0; i < 12; i++)
	{
		FracRenderVector3 z_clamped = clamp_vector_3(z, size_clamp_min, size_clamp_max);
		z.x = (2.f * z_clamped.x) - z.x;
		z.y = (2.f * z_clamped.y) - z.y;
		z.z = (2.f * z_clamped.z) - z.z;
		float r2 = dot(z, z);
		float k = fmax(2.f / r2, 0.027f);
		z = multiply_vector_3_scalar(z, k);
		scale *= k;
	}

	float l;
	if ((z.x == 0.f) && (z.y == 0.f)) { l = 0.f; }
	else { l = sqrt((z.x * z.x) + (z.y * z.y)); }
	float rxy = l - 4.f;
	float n = l * z.z;
	rxy = fmax(rxy, -n / 4.f);

	return rxy / fabs(scale);
}

// Print out a few voxels for debugging:
void print_sdf_3d_voxels(FracRenderSDF3D *sdf_3d)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Voxel debugging data for 3D SDF\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	printf("Number of voxels: %d.\n\n", sdf_3d->num_voxels);

	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	printf("Dimensions (min to max):\n");
	printf("X: %f to %f.\n", sdf_3d->centre.x - sdf_3d->size, sdf_3d->centre.x + sdf_3d->size);
	printf("Y: %f to %f.\n", sdf_3d->centre.y - sdf_3d->size, sdf_3d->centre.y + sdf_3d->size);
	printf("Z: %f to %f.\n", sdf_3d->centre.z - sdf_3d->size, sdf_3d->centre.z + sdf_3d->size);

	uint32_t index = 0;
	FracRenderVector3 centre = sdf_3d->centre;
	float size = sdf_3d->size;
	int cube_index = 1;

	printf("Level: 0\n");
	printf("Centre: %f, %f, %f\n", centre.x, centre.y, centre.z);
	printf("Size: %f\n", size);

	for (uint32_t i = 1; i < sdf_3d->levels; i++)
	{
		printf("----------------------------------------");
		printf("----------------------------------------\n\n");

		// Get upper top-right cube (index 1 in the row of 8):
		size /= 2.f;
		centre.x += size;
		centre.y += size;
		centre.z += size;

		index += pow(8, sdf_3d->levels - i) * cube_index;

		printf("Level: %d\n", i);
		printf("Centre: %f, %f, %f\n", centre.x, centre.y, centre.z);
		printf("Size: %f\n\n", size);
	}

	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	size /= 2.f;
	centre.x += size;
	centre.y += size;
	centre.z += size;

	printf("Level: %d\n", sdf_3d->levels);
	printf("Centre: %f, %f, %f\n", centre.x, centre.y, centre.z);
	printf("Size: %f\n", size);
	printf("Index: %d\n", index + cube_index);
	printf("Distance: %f\n\n", sdf_3d->voxels[index]);

	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
}
