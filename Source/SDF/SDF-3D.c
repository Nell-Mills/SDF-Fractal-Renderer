#include "SDF-3D.h"

// Calculate 3D SDF:
int create_sdf_3d(FracRenderSDF3D *sdf_3d)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Creating 3D SDF...\n");

	// Allocate memory for the SDF:
	printf(" ---> Allocating memory.\n");
	printf("      - Size of a voxel: %lu bytes.\n", sizeof(FracRenderVoxel));
	size_t memory_required = sdf_3d->num_voxels * sizeof(FracRenderVoxel);

	if ((memory_required / (1024 * 1024)) > 2000)
	{
		fprintf(stderr, "Error: Tried to allocate more than 2GB!\n");
		return -1;
	}

	sdf_3d->voxels = malloc(memory_required);
	printf("      - Memory allocated: %lu bytes (%lu MB).\n", memory_required,
						memory_required / (1024 * 1024));

	// First voxel occupies index 0 so first safe index is 1:
	printf(" ---> Calculating distance values.\n");
	uint32_t safe_index = 1;
	if (create_sdf_3d_helper(sdf_3d, sdf_3d->size, sdf_3d->centre, 0, &safe_index, 0) != 0)
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
			uint32_t current_index, uint32_t *safe_index, uint32_t level)
{
	// Check index:
	if (current_index >= sdf_3d->num_voxels)
	{
		fprintf(stderr, "Error: 3D SDF tried to exceed memory allocation!\n");
		return -1;
	}

	// Get centre of voxel:
	sdf_3d->voxels[current_index].centre = centre;

	// Calculate signed distance from surface:
	sdf_3d->voxels[current_index].distance = signed_distance_function(centre);

	// Get size of voxel (inflate a little to account for floating point imprecision):
	sdf_3d->voxels[current_index].size = size + 0.00001f;

	if (level > 2)
	{
		// Max resolution for SDF (2 levels for 8^2 = 64 voxels along each side):
		sdf_3d->voxels[current_index].num_subvoxels = 0;
		sdf_3d->voxels[current_index].first_subvoxel_index = 0;
	}
	else
	{
		FracRenderVector3 new_centre;
		sdf_3d->voxels[current_index].num_subvoxels = 8;
		sdf_3d->voxels[current_index].first_subvoxel_index = *safe_index;

		// Split cube into 8 smaller cubes and increment safe index by 8:
		current_index = *safe_index;
		*safe_index += 8;

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
			current_index, safe_index, level + 1) != 0)
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
			current_index + 1, safe_index, level + 1) != 0)
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
			current_index + 2, safe_index, level + 1) != 0)
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
			current_index + 3, safe_index, level + 1) != 0)
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
			current_index + 4, safe_index, level + 1) != 0)
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
			current_index + 5, safe_index, level + 1) != 0)
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
			current_index + 6, safe_index, level + 1) != 0)
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
			current_index + 7, safe_index, level + 1) != 0)
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

// Signed distance function:
float signed_distance_function(FracRenderVector3 position)
{
	FracRenderVector3 w = position;
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
		float a = 8.f * atan2(w.z, w.x);
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
void print_sdf_3d_voxels(FracRenderSDF3D *sdf_3d)
{
	printf("----------------------------------------");
	printf("----------------------------------------\n");
	printf("Voxel debugging data for 3D SDF\n");
	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	printf("Max number of voxels:\t\t134217728.\n");
	printf("Actual number of voxels:\t %d.\n\n", sdf_3d->num_voxels);

	printf("----------------------------------------");
	printf("----------------------------------------\n\n");

	printf("Dimensions (min to max):\n");
	printf("X: %f to %f.\n", sdf_3d->centre.x - sdf_3d->size, sdf_3d->centre.x + sdf_3d->size);
	printf("Y: %f to %f.\n", sdf_3d->centre.y - sdf_3d->size, sdf_3d->centre.y + sdf_3d->size);
	printf("Z: %f to %f.\n", sdf_3d->centre.z - sdf_3d->size, sdf_3d->centre.z + sdf_3d->size);

	uint32_t index = 0;
	for (int i = 0; i < 3; i++)
	{
		printf("----------------------------------------");
		printf("----------------------------------------\n\n");

		if (i > 0)
		{
			index = sdf_3d->voxels[index].first_subvoxel_index;
		}

		printf("Voxel %d:\n", index);
		printf("Centre: %f, %f, %f\n", sdf_3d->voxels[index].centre.x,
						sdf_3d->voxels[index].centre.y,
						sdf_3d->voxels[index].centre.z);
		printf("Distance: %f\n", sdf_3d->voxels[index].distance);
		printf("Number of sub-voxels: %d\n", sdf_3d->voxels[index].num_subvoxels);
		printf("First sub-voxel index: %d\n", sdf_3d->voxels[index].first_subvoxel_index);
		printf("Size: %f\n\n", sdf_3d->voxels[index].size);
	}

	printf("----------------------------------------");
	printf("----------------------------------------\n\n");
}
