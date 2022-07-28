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
	// (vec4, 4 bytes) Location of centre of voxel and its distance value:
	FracRenderVector3 centre;
	float distance;

	// (uvec2, 2 bytes) Subvoxels:
	uint32_t num_subvoxels;
	uint32_t first_subvoxel_index;

	// (vec2, 2 bytes) Size (centre to edge) and max index:
	float size;
	float max_index; // Needed an extra value to pad so whole struct is 64 bits.
} FracRenderVoxel;

typedef struct {
	// Number of voxels:
	uint32_t num_voxels;

	// Dimensions:
	float min_x;
	float max_x;
	float min_y;
	float max_y;
	float min_z;
	float max_z;

	// Voxels:
	FracRenderVoxel *voxels;
} FracRenderSDF;

/***********************
 * Function Prototypes *
 ***********************/

// Calculate how many voxels are needed:
void calculate_memory(FracRenderSDF *sdf);

// Recursion helper for memory requirement calculation:
uint32_t calculate_memory_helper(float min_x, float max_x, float min_y,
		float max_y, float min_z, float max_z, uint32_t level);

// Calculate SDF:
void create_sdf(FracRenderSDF *sdf);

// SDF recursion helper:
void create_sdf_helper(FracRenderSDF *sdf, float min_x, float max_x, float min_y, float max_y,
	float min_z, float max_z, uint32_t current_index, uint32_t *safe_index, uint32_t level);

// Free SDF memory:
void destroy_sdf(FracRenderSDF *sdf);

// Signed distance function:
float signed_distance_function(FracRenderVector3 centre);

// Print out a few voxels for debugging:
void print_voxels(FracRenderSDF *sdf);

#endif
