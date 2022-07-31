#ifndef FRACRENDER_SDF_3D_H
#define FRACRENDER_SDF_3D_H

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
} FracRenderSDF3D;

/***********************
 * Function Prototypes *
 ***********************/

// Calculate how many voxels are needed:
void calculate_sdf_3d_voxels(FracRenderSDF3D *sdf_3d);

// Recursion helper for memory requirement calculation:
uint32_t calculate_sdf_3d_voxels_helper(float size, FracRenderVector3 centre, uint32_t level);

// Calculate 3D SDF:
int create_sdf_3d(FracRenderSDF3D *sdf_3d);

// SDF recursion helper:
int create_sdf_3d_helper(FracRenderSDF3D *sdf_3d, float size, FracRenderVector3 centre,
			uint32_t current_index, uint32_t *safe_index, uint32_t level);

// Free SDF memory:
void destroy_sdf_3d(FracRenderSDF3D *sdf_3d);

// Signed distance function:
float signed_distance_function(FracRenderVector3 position);

// Print out a few voxels for debugging:
void print_sdf_3d_voxels(FracRenderSDF3D *sdf_3d);

#endif