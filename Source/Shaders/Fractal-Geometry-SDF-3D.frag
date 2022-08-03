#version 450

layout (location = 0) in vec4 in_position;

layout (set = 0, binding = 0) uniform UScene
{
	vec3 plane_centre;
	vec3 x_axis;
	vec3 y_axis;
	vec3 eye_position;
	float aspect_ratio;
	float mandelbulb_parameter;
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

// Function prototypes:
vec4 raymarch(vec3 origin, vec3 ray);
vec2 distance_lookup(vec3 position);
float distance_estimator_mandelbulb(vec3 position);
float distance_estimator_hall_of_pillars(vec3 position);
float ray_cube(vec3 origin, vec3 ray, float cube_size);

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
	vec2 distance_estimate_vec2 = distance_lookup(origin);

	// If cube size is invalid, point is not in main cube. Move ray along:
	if (distance_estimate_vec2.y < 0.f)
	{
		distance_estimate_vec2.x = ray_cube(origin, ray, b_voxels.voxels[0].size.x);
		if (distance_estimate_vec2.x < 0.f)
		{
			// Ray does not intersect main SDF cube:
			return vec4(-1.f);
		}
		distance_travelled += distance_estimate_vec2.x * 1.01f;
	}

	// Get current position and distance travelled again:
	current_position = vec4(origin + (ray * distance_travelled), 1.f);
	distance_estimate_vec2 = distance_lookup(current_position.xyz);
	distance_travelled += distance_estimate_vec2.x;

	for (int steps_taken = 0; steps_taken <= max_steps; steps_taken++)
	{
		// Get current position. Encode iterations in w-coordinate:
		current_position = vec4(origin + (ray * distance_travelled),
			1.f - (float(steps_taken) / float(max_steps)));

		// Get distance estimate and update total distance travelled:
		distance_estimate = distance_estimator_mandelbulb(current_position.xyz);
		//distance_estimate = distance_estimator_hall_of_pillars(current_position.xyz);
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

float distance_estimator_mandelbulb(vec3 position)
{
	int max_iterations = 4;
	float escape_radius = 2.f;
	float parameter = u_scene.mandelbulb_parameter;

	vec3 z = position;	// Z = Z^2 + C.
	float dr = 1.f;
	float r = 0.0;		// Radius.

	for (int i = 0; i < max_iterations; i++)
	{
		r = length(z);
		if (r > escape_radius) { break; }

		// Convert position to polar coordinates:
		float theta = acos(z.z / r);
		float phi = atan(z.y, z.x);
		dr = (pow(r, parameter - 1.f) * parameter * dr) + 1.f;

		// Scale and rotate position:
		float zr = pow(r, parameter);
		theta *= parameter;
		phi *= parameter;

		// Convert position back to Cartesian coordinates:
		z = (zr * vec3(sin(theta) * cos(phi), sin(phi) * sin(theta),
						cos(theta))) + position;
	}

	// Calculate distance:
	return 0.5f * log(r) * (r / dr);
}

float distance_estimator_hall_of_pillars(vec3 position)
{
        vec3 z = position.xzy;
        float scale = 1.f;
        vec3 size_clamp = vec3(1.f, 1.f, 1.3f);

        for (int i = 0; i < 12; i++)
        {
                z = 2.f * clamp(z, -size_clamp, size_clamp) - z;
                float r2 = dot(z, z);
                float k = max(2.f / r2, 0.027f);
                z *= k;
                scale *- k;
        }

        float l = length(z.xy);
        float rxy = l - 4.f;
        float n = l * z.z;
        rxy = max(rxy, -n / 4.f);

        return rxy / abs(scale);
}

float ray_cube(vec3 origin, vec3 ray, float cube_size)
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
