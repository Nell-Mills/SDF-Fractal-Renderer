#version 450

layout (location = 0) in vec4 in_position;

layout (set = 0, binding = 0) uniform UScene
{
	vec3 plane_centre;
	vec3 x_axis;
	vec3 y_axis;
	vec3 eye_position;
	float aspect_ratio;
} u_scene;

struct SVoxel
{
	// Centre - xyz, Distance - w:
	vec4 centre;

	// Number of subvoxels - x, first subvoxel index - y:
	uvec2 subvoxels;

	// Size - x, padding - y:
	vec2 size;
};

layout (set = 1, binding = 0) buffer BVoxels
{
	SVoxel voxels[];
} b_voxels;

layout (location = 0) out vec4 out_position;
layout (location = 1) out vec4 out_normal;

// Function prototypes:
vec4 raymarch(vec3 origin, vec3 ray);
float distance_estimator(vec3 position);

// Main function:
void main()
{
	// Get ray from eye to pixel:
	vec3 ray = in_position.xyz - u_scene.eye_position;
	ray = normalize(ray);

	// Find closest point on Mandelbulb and calculate normal:
	//out_position = raymarch(in_position.xyz, ray);
	out_position = vec4(b_voxels.voxels[0].centre.xyz, 1.f);
}

vec4 raymarch(vec3 origin, vec3 ray)
{
	vec4 current_position;
	int max_steps = 50;
	float distance_travelled = 0.f;
	float distance_threshold = 0.0001f;

	for (int steps_taken = 0; steps_taken < max_steps; steps_taken++)
	{
		// Get current position. Encode iterations in w-coordinate:
		current_position = vec4(origin + (ray * distance_travelled),
			1.f - (float(steps_taken) / float(max_steps)));

		// Get distance (under)estimate and update total distance travelled:
		float distance_estimate = distance_estimator(current_position.xyz);
		distance_travelled += distance_estimate * 0.9f;

		// Check how close the point is to the surface:
		if (distance_estimate < distance_threshold)
		{
			break;
		}
	}

	// Calculate normal based on distance gradient:
	out_normal = vec4(0.f, 1.f, 1.f, 1.f);

	// Return current position along with iterations achieved:
	return current_position;
}

float distance_estimator(vec3 position)
{
	uint index = 0;		// Current index.
	uint parent = 0;	// Index of parent of current.
	uint counter = 0;	// How many children of parent we have looked at.

	// Find out which cube the point is in:
	while (true)
	{
		if ((position.x > (b_voxels.voxels[index].centre.x - b_voxels.voxels[index].size.x)
		&& (position.x < (b_voxels.voxels[index].centre.x + b_voxels.voxels[index].size.x))
		&& (position.y > (b_voxels.voxels[index].centre.y - b_voxels.voxels[index].size.x))
		&& (position.y < (b_voxels.voxels[index].centre.y + b_voxels.voxels[index].size.x))
		&& (position.z > (b_voxels.voxels[index].centre.z - b_voxels.voxels[index].size.x))
		&& (position.z < (b_voxels.voxels[index].centre.z +
				b_voxels.voxels[index].size.x))))
		{
			if (b_voxels.voxels[index].subvoxels.x == 0)
			{
				// Lowest resolution voxel available, break:
				break;
			}
			else
			{
				// Go to subvoxels:
				index = b_voxels.voxels[index].subvoxels.y;
				counter = 0;
			}

		}
		else
		{
			index++;
		}
	}

	// Distance is encoded in voxel centre w-coordinate:
	return b_voxels.voxels[index].centre.w;
}
