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

layout (set = 1, binding = 0) readonly buffer BVoxels
{
	SVoxel voxels[];
} b_voxels;

layout (location = 0) out vec4 out_position;
layout (location = 1) out vec4 out_normal;

// Function prototypes:
vec4 raymarch(vec3 origin, vec3 ray);
vec2 distance_lookup(vec3 position);
float ray_cube_intersect(vec3 origin, vec3 ray, float cube_size);

// Main function:
void main()
{
	// Get ray from eye to pixel:
	vec3 ray = in_position.xyz - u_scene.eye_position;
	ray = normalize(ray);

	// Find closest point on Mandelbulb and calculate normal:
	out_position = raymarch(in_position.xyz, ray);
}

vec4 raymarch(vec3 origin, vec3 ray)
{
	vec4 current_position;
	int max_steps = 49;
	float distance_travelled = 0.f;
	float distance_threshold = 0.003f;

	{
		// Get current position. Encode iterations in w-coordinate:
		current_position = vec4(origin + (ray * distance_travelled), 1.f);

		// Look up distance estimate and update total distance travelled:
		vec2 distance_estimate = distance_lookup(current_position.xyz);

		// If cube size is invalid, point is not in main cube. Move ray along:
		if (distance_estimate.y < 0.f)
		{
			distance_estimate.x = ray_cube_intersect(origin, ray,
						b_voxels.voxels[0].size.x);
			if (distance_estimate.x < 0.f)
			{
				// Ray does not intersect main SDF cube:
				return vec4(-1.f);
			}
			distance_travelled += distance_estimate.x * 1.01f;
		}
	}

	for (int steps_taken = 0; steps_taken <= max_steps; steps_taken++)
	{
		// Get current position. Encode iterations in w-coordinate:
		current_position = vec4(origin + (ray * distance_travelled),
			1.f - (float(steps_taken) / float(max_steps)));

		// Look up distance estimate and update total distance travelled:
		vec2 distance_estimate = distance_lookup(current_position.xyz);

		// Update distance travelled:
		distance_travelled += distance_estimate.x;

		// If distance is less than cube size or threshold, point is close enough:
		if ((distance_estimate.x < distance_estimate.y) ||
			(abs(distance_estimate.x) < distance_threshold))
		{
			current_position = vec4(origin + (ray * distance_travelled),
				1.f - (float(steps_taken) / float(max_steps)));
			break;
		}
	}

	// Calculate normal based on distance gradient:
	out_normal = vec4(0.f, 1.f, 0.f, 1.f);

	// Return current position along with iterations achieved:
	return current_position;
}

vec2 distance_lookup(vec3 position)
{
	uint index = 0;		// Current index.
	uint counter = 0;	// How many children of parent have been looked at.

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
				// Lowest resolution voxel available, return:
				return vec2(b_voxels.voxels[index].centre.w,
						b_voxels.voxels[index].size.x);
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
			counter++;
			if ((counter > 7) || (index == 0)) {
				// Point is not found, return invalid value:
				return vec2(-1.f);
			}
			index++;
		}
	}
}

float ray_cube_intersect(vec3 origin, vec3 ray, float cube_size)
{
	// Function from: https://iquilezles.org/articles/intersectors/
	vec3 m = 1.f / ray;
	vec3 n = m * origin;
	vec3 k = abs(m) * vec3(cube_size);
	vec3 t1 = -n - k;
	vec3 t2 = -n + k;
	float tN = max(max(t1.x, t1.y), t1.z);
	float tF = min(min(t2.x, t2.y), t2.z);
	if ((tN > tF) || (tF < 0.f)) { return -1.f; }	// Doesn't intersect.
	return tN;	// Nearest intersection distance.
}
