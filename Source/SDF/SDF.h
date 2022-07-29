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
	// (vec4, 16 bytes) Location of centre of voxel and its distance value:
	FracRenderVector3 centre;
	float distance;

	// (uvec2, 8 bytes) Subvoxels:
	uint32_t num_subvoxels;
	uint32_t first_subvoxel_index;

	// (vec2, 8 bytes) Size (centre to edge) and a padding value:
	float size;
	float pad;
} FracRenderVoxel;

typedef struct {
	// Number of voxels:
	uint32_t num_voxels;

	// Cube size and centre:
	float size;
	FracRenderVector3 centre;

	// Voxels:
	FracRenderVoxel *voxels;
} FracRenderSDF;

/***********************
 * Function Prototypes *
 ***********************/

// Calculate how many voxels are needed:
void calculate_memory(FracRenderSDF *sdf);

// Recursion helper for memory requirement calculation:
uint32_t calculate_memory_helper(float size, FracRenderVector3 centre, uint32_t level);

// Calculate SDF:
int create_sdf(FracRenderSDF *sdf);

// SDF recursion helper:
int create_sdf_helper(FracRenderSDF *sdf, float size, FracRenderVector3 centre,
		uint32_t current_index, uint32_t *safe_index, uint32_t level);

// Free SDF memory:
void destroy_sdf(FracRenderSDF *sdf);

// Signed distance function:
float signed_distance_function(FracRenderVector3 position);

// Print out a few voxels for debugging:
void print_voxels(FracRenderSDF *sdf);

#endif
