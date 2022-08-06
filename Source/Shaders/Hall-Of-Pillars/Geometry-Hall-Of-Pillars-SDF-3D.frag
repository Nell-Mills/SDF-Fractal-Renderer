#version 450

layout (location = 0) in vec4 in_position;

layout (set = 0, binding = 0) uniform UScene
{
	// Axes in eye coordinate system:
	vec3 plane_centre;
	vec3 x_axis;
	vec3 y_axis;

	// Eye position:
	vec3 eye_position;

	// 3D SDF information:
	vec3 sdf_3d_centre;
	float sdf_3d_size;
	uint sdf_3d_levels;

	// Aspect ratio:
	float aspect_ratio;

	// Fractal parameter:
	float fractal_parameter;
} u_scene;

layout (set = 1, binding = 0) readonly buffer BVoxels
{
	float voxels[];
} b_voxels;

layout (location = 0) out vec4 out_position;

// Function prototypes:
vec4 raymarch(vec3 origin, vec3 ray);
vec2 sdf_3d_lookup(vec3 position);
bool in_cube(vec3 cube_centre, float cube_size, vec3 point);
float distance_estimator_hall_of_pillars(vec3 position);

// Main function:
void main()
{
	// Get ray from eye to pixel:
	vec3 ray = in_position.xyz - u_scene.eye_position;
	ray = normalize(ray);

	// Find closest point on Mandelbulb and calculate normal:
	out_position = raymarch(u_scene.eye_position, ray);
}

vec4 raymarch(vec3 origin, vec3 ray)
{
	vec4 current_position;
	int max_steps = 999;
	float distance_estimate;
	float distance_travelled = 0.f;
	float distance_threshold = 0.0001f;

	// Look up distance estimate and update total distance travelled:
	vec2 distance_lookup = sdf_3d_lookup(origin);

	// Check that ray is in main SDF cube and update distance:
	if (distance_lookup.y > 0.f)
	{
		distance_travelled += distance_lookup.x;
	}

	for (int steps_taken = 0; steps_taken <= max_steps; steps_taken++)
	{
		// Get current position. Encode iterations in w-coordinate:
		current_position = vec4(origin + (ray * distance_travelled),
			1.f - (float(steps_taken) / float(max_steps)));

		// Get distance estimate and update total distance travelled:
		distance_estimate = distance_estimator_hall_of_pillars(current_position.xyz);
		distance_travelled += distance_estimate;

		// Check how close the point is to the surface:
		if (distance_estimate < distance_threshold)
		{
			current_position = vec4(origin + (ray * distance_travelled),
				1.f - (float(steps_taken) / float(max_steps)));
			break;
		}
	}

	// Return current position along with iterations achieved:
	return current_position;
}

vec2 sdf_3d_lookup(vec3 position)
{
	// Ordering of sub-cubes (looking top-down, upper/lower is y-coordinate):
	vec3 sub_cubes[] = {
		vec3(-1.f,  1.f,  1.f),	// Upper, top-left.
		vec3( 1.f,  1.f,  1.f),	// Upper, top-right.
		vec3(-1.f,  1.f, -1.f),	// Upper, bottom-left.
		vec3( 1.f,  1.f, -1.f),	// Upper, bottom-right.
		vec3(-1.f, -1.f,  1.f),	// Lower, top-left.
		vec3( 1.f, -1.f,  1.f),	// Lower, top-right.
		vec3(-1.f, -1.f, -1.f),	// Lower, bottom-left.
		vec3( 1.f, -1.f, -1.f)	// Lower, bottom-right.
	};

	uint index = 0;
	vec3 centre = u_scene.sdf_3d_centre;
	float size = u_scene.sdf_3d_size;

	// Check if point is in main cube:
	if (!in_cube(centre, size, position))
	{
		return vec2(0.f, -1.f);
	}

	// Find out which cube the point is in:
	uint level = 1;
	uint max_level = u_scene.sdf_3d_levels;
	while (true)
	{
		// Cut size in two:
		size /= 2.f;

		for (uint i = 0; i < 8; i++)
		{
			vec3 new_centre = centre + (sub_cubes[i] * size);
			if (in_cube(new_centre, size, position))
			{
				if (level == max_level) {
					return vec2(b_voxels.voxels[index + i], 1.f);
				}

				// Move to sub-cubes:
				centre = new_centre;
				index += uint(pow(8.f, float(max_level) - float(level))) * i;
				break;
			}
		}

		if (level == max_level) { return vec2(0.f, -1.f); }
		level++;
	}
}

bool in_cube(vec3 cube_centre, float cube_size, vec3 point)
{
	if (	(point.x >= (cube_centre.x - cube_size)) &&
		(point.x <= (cube_centre.x + cube_size)) &&
		(point.y >= (cube_centre.y - cube_size)) &&
		(point.y <= (cube_centre.y + cube_size)) &&
		(point.z >= (cube_centre.z - cube_size)) &&
		(point.z <= (cube_centre.z + cube_size)))
	{
		return true;
	}

	return false;
}

float distance_estimator_hall_of_pillars(vec3 position)
{
        vec3 z = position.xzy;
        float scale = 1.f;
        vec3 size_clamp = vec3(1.f, 1.f, 1.3f);

        for (int i = 0; i < 12; i++)
        {
                z = (2.f * clamp(z, -size_clamp, size_clamp)) - z;
                float r2 = dot(z, z);
                float k = max(2.f / r2, 0.027f);
                z *= k;
                scale *= k;
        }

        float l = length(z.xy);
        float rxy = l - 4.f;
        float n = l * z.z;
        rxy = max(rxy, -n / 4.f);

        return rxy / abs(scale);
}
