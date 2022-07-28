#include "SDF.h"

// Calculate how many voxels are needed:
void calculate_memory(FracRenderSDF *sdf)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Calculating memory requirements of SDF...\n");

	sdf->num_voxels = calculate_memory_helper(sdf->min_x, sdf->max_x,
			sdf->min_y, sdf->max_y, sdf->min_z, sdf->max_z, 0);

	printf(" ---> Voxels needed: %d\n", sdf->num_voxels);

	sdf->num_voxels += (uint32_t)((float)(sdf->num_voxels) * 0.25f);

	printf(" ---> Will allocate for %d to account for floating "
				"point inaccuracy.\n", sdf->num_voxels);

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
}

// Recursion helper for memory requirement calculation:
uint32_t calculate_memory_helper(float min_x, float max_x, float min_y,
		float max_y, float min_z, float max_z, uint32_t level)
{
	// Max resolution for SDF (9 levels for 2^9 = 512 voxels along each side):
	if (level > 8) { return 1; }

	// Calculate centre of voxel:
	FracRenderVector3 centre;
	centre.x = (max_x + min_x) / 2.f;
	centre.y = (max_y + min_y) / 2.f;
	centre.z = (max_z + min_z) / 2.f;

	// Calculate signed distance from surface:
	float distance = signed_distance_function(centre);

	if (fabs(distance) > ((sqrt(3) * fabs(max_x - min_x) * 0.5f) + 0.01f))
	{
		// If distance is larger than diagonal cube length, don't split:
		return 1;
	}
	else
	{
		// Split cube into 8 smaller cubes:
		uint32_t result = 0;

		// Upper, top-left cube (looking top-down):
		result += calculate_memory_helper(
				min_x, (max_x + min_x) / 2.f,
				(max_y + min_y) / 2.f, max_y,
				(max_z + min_z) / 2.f, max_z,
				level + 1
		);

		// Upper, top-right cube:
		result += calculate_memory_helper(
				(max_x + min_x) / 2.f, max_x,
				(max_y + min_y) / 2.f, max_y,
				(max_z + min_z) / 2.f, max_z,
				level + 1
		);

		// Upper, bottom-left cube:
		result += calculate_memory_helper(
				min_x, (max_x + min_x) / 2.f,
				(max_y + min_y) / 2.f, max_y,
				min_z, (max_z + min_z) / 2.f,
				level + 1
		);

		// Upper, bottom-right cube:
		result += calculate_memory_helper(
				(max_x + min_x) / 2.f, max_x,
				(max_y + min_y) / 2.f, max_y,
				min_z, (max_z + min_z) / 2.f,
				level + 1
		);

		// Lower, top-left cube (looking top-down):
		result += calculate_memory_helper(
				min_x, (max_x + min_x) / 2.f,
				min_y, (max_y + min_y) / 2.f,
				(max_z + min_z) / 2.f, max_z,
				level + 1
		);

		// Lower, top-right cube:
		result += calculate_memory_helper(
				(max_x + min_x) / 2.f, max_x,
				min_y, (max_y + min_y) / 2.f,
				(max_z + min_z) / 2.f, max_z,
				level + 1
		);

		// Lower, bottom-left cube:
		result += calculate_memory_helper(
				min_x, (max_x + min_x) / 2.f,
				min_y, (max_y + min_y) / 2.f,
				min_z, (max_z + min_z) / 2.f,
				level + 1
		);

		// Lower, bottom-right cube:
		result += calculate_memory_helper(
				(max_x + min_x) / 2.f, max_x,
				min_y, (max_y + min_y) / 2.f,
				min_z, (max_z + min_z) / 2.f,
				level + 1
		);

		return result;
	}
}

// Calculate SDF:
void create_sdf(FracRenderSDF *sdf)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Creating SDF...\n");

	// Allocate memory for the SDF:
	printf(" ---> Allocating memory.\n");
	size_t memory_required = sdf->num_voxels * sizeof(FracRenderVoxel);
	sdf->voxels = malloc(memory_required);
	printf("      - Memory allocated: %lu bytes (%lu MB).\n", memory_required,
						memory_required / (1024 * 1024));

	// First voxel occupies index 0 so first safe index is 1:
	printf(" ---> Calculating distance values.\n");
	uint32_t safe_index = 1;
	create_sdf_helper(sdf, sdf->min_x, sdf->max_x, sdf->min_y, sdf->max_y,
				sdf->min_z, sdf->max_z, 0, &safe_index, 0);

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
}

// SDF recursion helper:
void create_sdf_helper(FracRenderSDF *sdf, float min_x, float max_x, float min_y, float max_y,
	float min_z, float max_z, uint32_t current_index, uint32_t *safe_index, uint32_t level)
{
	// Check index:
	if (current_index >= sdf->num_voxels)
	{
		fprintf(stderr, "Warning: SDF tried to exceed memory allocation!\n");
	}

	// Calculate centre of voxel:
	sdf->voxels[current_index].centre.x = (max_x + min_x) / 2.f;
	sdf->voxels[current_index].centre.y = (max_y + min_y) / 2.f;
	sdf->voxels[current_index].centre.z = (max_z + min_z) / 2.f;

	// Calculate signed distance from surface:
	sdf->voxels[current_index].distance = signed_distance_function(
					sdf->voxels[current_index].centre);

	// Calculate size of voxel:
	sdf->voxels[current_index].size = fabs((max_x - min_x) / 2.f);

	// Record max voxels:
	sdf->voxels[current_index].max_index = (float)(sdf->num_voxels);

	if (fabs(sdf->voxels[current_index].distance) > ((sqrt(3) * fabs(max_x - min_x) * 0.5f) + 0.01f))
	{
		// If distance is larger than diagonal cube length, don't split:
		sdf->voxels[current_index].num_subvoxels = 0;
		sdf->voxels[current_index].first_subvoxel_index = 0;
	}
	else if (level > 8) {
		// Max resolution for SDF (9 levels for 2^9 = 512 voxels along each side):
		sdf->voxels[current_index].num_subvoxels = 0;
		sdf->voxels[current_index].first_subvoxel_index = 0;
	}
	else
	{
		sdf->voxels[current_index].num_subvoxels = 8;
		sdf->voxels[current_index].first_subvoxel_index = *safe_index;

		// Split cube into 8 smaller cubes and increment safe index by 8:
		current_index = *safe_index;
		*safe_index += 8;

		// Print progress:
		if (level == 0)
		{
			printf("      --->   0.0%%.\n");
		}

		// Upper, top-left cube (looking top-down):
		create_sdf_helper(
				sdf, min_x, (max_x + min_x) / 2.f,
				(max_y + min_y) / 2.f, max_y,
				(max_z + min_z) / 2.f, max_z,
				current_index, safe_index, level + 1
		);

		// Print progress:
		if (level == 0)
		{
			printf("      --->  12.5%%.\n");
		}

		// Upper, top-right cube:
		create_sdf_helper(
				sdf, (max_x + min_x) / 2.f, max_x,
				(max_y + min_y) / 2.f, max_y,
				(max_z + min_z) / 2.f, max_z,
				current_index + 1, safe_index, level + 1
		);

		// Print progress:
		if (level == 0)
		{
			printf("      --->  25.0%%.\n");
		}

		// Upper, bottom-left cube:
		create_sdf_helper(
				sdf, min_x, (max_x + min_x) / 2.f,
				(max_y + min_y) / 2.f, max_y,
				min_z, (max_z + min_z) / 2.f,
				current_index + 2, safe_index, level + 1
		);

		// Print progress:
		if (level == 0)
		{
			printf("      --->  37.5%%.\n");
		}

		// Upper, bottom-right cube:
		create_sdf_helper(
				sdf, (max_x + min_x) / 2.f, max_x,
				(max_y + min_y) / 2.f, max_y,
				min_z, (max_z + min_z) / 2.f,
				current_index + 3, safe_index, level + 1
		);

		// Print progress:
		if (level == 0)
		{
			printf("      --->  50.0%%.\n");
		}

		// Lower, top-left cube (looking top-down):
		create_sdf_helper(
				sdf, min_x, (max_x + min_x) / 2.f,
				min_y, (max_y + min_y) / 2.f,
				(max_z + min_z) / 2.f, max_z,
				current_index + 4, safe_index, level + 1
		);

		// Print progress:
		if (level == 0)
		{
			printf("      --->  62.5%%.\n");
		}

		// Lower, top-right cube:
		create_sdf_helper(
				sdf, (max_x + min_x) / 2.f, max_x,
				min_y, (max_y + min_y) / 2.f,
				(max_z + min_z) / 2.f, max_z,
				current_index + 5, safe_index, level + 1
		);

		// Print progress:
		if (level == 0)
		{
			printf("      --->  75.0%%.\n");
		}

		// Lower, bottom-left cube:
		create_sdf_helper(
				sdf, min_x, (max_x + min_x) / 2.f,
				min_y, (max_y + min_y) / 2.f,
				min_z, (max_z + min_z) / 2.f,
				current_index + 6, safe_index, level + 1
		);

		// Print progress:
		if (level == 0)
		{
			printf("      --->  87.5%%.\n");
		}

		// Lower, bottom-right cube:
		create_sdf_helper(
				sdf, (max_x + min_x) / 2.f, max_x,
				min_y, (max_y + min_y) / 2.f,
				min_z, (max_z + min_z) / 2.f,
				current_index + 7, safe_index, level + 1
		);

		// Print progress:
		if (level == 0)
		{
			printf("      ---> 100.0%%.\n");
		}
	}
}

// Free SDF memory:
void destroy_sdf(FracRenderSDF *sdf)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Freeing memory for SDF (CPU-side)...\n");

	if (sdf->voxels)
	{
		free(sdf->voxels);
	}

	printf("... Done.\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
}

// Signed distance function:
float signed_distance_function(FracRenderVector3 centre)
{
	FracRenderVector3 w = centre;
	if ((w.x == 0.f) && (w.y == 0.f) && (w.z == 0.f))
	{
		w.x = 0.0001f;
		w.y = 0.0001f;
		w.z = 0.0001f;
	}
	float m = dot(w, w);
	float dz = 1.f;

	for (int i = 0; i < 4; i++)
	{
		float root_m = sqrt(m);
		dz = (8.f * pow(root_m, 7.f) * dz) + 1.f;

		float r = length(w);
		float b;
		if (r == 0.f) { b = 8.f * acos(0.f); }
		else { b = 8.f * acos(w.y / r); }
		float a = 8.f * atan2(w.x, w.z);
		float r_8 = pow(r, 8.f);

		w.x = (sin(b) * sin(a) * r_8) + w.x;
		w.y = (cos(b) * r_8) + w.y;
		w.z = (sin(b) * cos(a) * r_8) + w.z;

		m = dot(w, w);

		if (m > 256.f) { break; }
	}

	return (0.25f * log(m) * sqrt(m)) / dz;
}

// Print out a few voxels for debugging:
void print_voxels(FracRenderSDF *sdf)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Voxel debugging data\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	printf("Max number of voxels:\t\t134217728.\n");
	printf("Actual number of voxels:\t %d.\n\n", sdf->num_voxels);

	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	printf("Dimensions (min to max):\n");
	printf("X: %f to %f.\n", sdf->min_x, sdf->max_x);
	printf("Y: %f to %f.\n", sdf->min_y, sdf->max_y);
	printf("Z: %f to %f.\n\n", sdf->min_z, sdf->max_z);

	uint32_t index = 0;
	for (int i = 0; i < 3; i++)
	{
		printf("----------------------------------------");
		printf("----------------------------------------\n\n");

		if (i > 0)
		{
			index = sdf->voxels[index].first_subvoxel_index;
		}

		printf("Voxel %d:\n", index);
		printf("Centre: %f, %f, %f\n", sdf->voxels[index].centre.x,
						sdf->voxels[index].centre.y,
						sdf->voxels[index].centre.z);
		printf("Distance: %f\n", sdf->voxels[index].distance);
		printf("Number of sub-voxels: %d\n", sdf->voxels[index].num_subvoxels);
		printf("First sub-voxel index: %d\n", sdf->voxels[index].first_subvoxel_index);
		printf("Size: %f\n\n", sdf->voxels[index].size);
	}

	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
}
