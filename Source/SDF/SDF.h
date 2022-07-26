#ifndef FRACRENDER_SDF_H
#define FRACRENDER_SDF_H

// Library includes:
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Local includes:
#include "../Utility/Vectors.h"

/**************
 * Structures *
 **************/

typedef struct {
	// (vec4) Location of centre of voxel and its distance value:
	FracRenderVector3 centre;
	float distance;

	// (uvec3) Subvoxels and size (centre to edge):
	uint32_t num_subvoxels;
	uint32_t first_subvoxel_index;
	float size;
} FracRenderVoxel;

typedef struct {
	// Voxels:
	FracRenderVoxel *voxels;
} FracRenderSDF;

/***********************
 * Function Prototypes *
 ***********************/

// Calculate how many voxels are needed:
uint32_t calculate_memory(float min_x, float max_x, float min_y,
			float max_y, float min_z, float max_z);

// Recursion helper for memory requirement calculation:
uint32_t calculate_memory_helper(float min_x, float max_x, float min_y,
		float max_y, float min_z, float max_z, uint32_t level);

// Calculate SDF:
void create_SDF(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z,
					FracRenderSDF *sdf, uint32_t sdf_memory_requirements);

// SDF recursion helper:
void create_SDF_helper(float min_x, float max_x, float min_y,
	float max_y, float min_z, float max_z, FracRenderSDF *sdf,
	uint32_t sdf_memory_requirements, uint32_t current_index,
	uint32_t *safe_index, uint32_t level);

// Free SDF memory:
void destroy_SDF(FracRenderSDF *sdf);

// Signed distance function:
float signed_distance_function(FracRenderVector3 centre);

// Print out a few voxels for debugging:
void print_voxels(FracRenderSDF *sdf);

#endif
