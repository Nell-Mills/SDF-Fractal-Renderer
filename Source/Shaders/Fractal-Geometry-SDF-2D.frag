#version 450

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec2 in_tex_coord;

layout (set = 0, binding = 0) uniform UScene
{
	vec3 plane_centre;
	vec3 x_axis;
	vec3 y_axis;
	vec3 eye_position;
	float mandelbulb_parameter;
} u_scene;

layout (set = 1, binding = 0) uniform sampler2D u_distance_sampler;

layout (location = 0) out vec4 out_position;
layout (location = 1) out vec4 out_normal;
layout (location = 2) out float out_distance;

// Function prototypes:
vec4 raymarch(vec3 origin, vec3 ray);
float distance_estimator_mandelbulb(vec3 position);
float distance_estimator_hall_of_pillars(vec3 position);
float distance_estimator_cubes(vec3 position);

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
/*	vec3 mandelbulb_positions[] = {
		vec3( 0.f, 0.f,  0.f),
		vec3(-1.f, 0.f,  1.f),
		vec3( 1.f, 0.f,  1.f),
		vec3(-1.f, 0.f, -1.f),
		vec3( 1.f, 0.f, -1.f)
	};
*/
	vec4 current_position;
	int max_steps = 999;
	float distance_estimate;
	float distance_travelled;
	float distance_threshold = 0.0001f;

	// Step according to written distance:
	float distance_0 = texture(u_distance_sampler, in_tex_coord).r;
	float distance_1 = textureOffset(u_distance_sampler, in_tex_coord, ivec2( 1, -1)).r;
	float distance_2 = textureOffset(u_distance_sampler, in_tex_coord, ivec2(-1, -1)).r;
	float distance_3 = textureOffset(u_distance_sampler, in_tex_coord, ivec2(-1,  1)).r;
	float distance_4 = textureOffset(u_distance_sampler, in_tex_coord, ivec2( 1,  1)).r;

	distance_travelled = min(min(min(min(distance_0, distance_1),
				distance_2), distance_3), distance_4);

	for (int steps_taken = 0; steps_taken <= max_steps; steps_taken++)
	{
		// Get current position. Encode iterations in w-coordinate:
		current_position = vec4(origin + (ray * distance_travelled),
			1.f - (float(steps_taken) / float(max_steps)));

		// Get distance estimate and update total distance travelled:
/*		float distance_estimates[5];
		for (int i = 0; i < 5; i++)
		{
			distance_estimates[i] = distance_estimator_mandelbulb_vertical(
					current_position.xyz + mandelbulb_positions[i]);
		}

		distance_estimate = min(min(min(min(distance_estimates[3],
			distance_estimates[4]), distance_estimates[2]),
			distance_estimates[1]), distance_estimates[0]);
*/
//		distance_estimate = distance_estimator_mandelbulb(current_position.xyz);
//		distance_estimate = distance_estimator_hall_of_pillars(current_position.xyz);
		distance_estimate = distance_estimator_cubes(current_position.xyz);
		distance_travelled += distance_estimate;

		// Check how close the point is to the surface:
		if (distance_estimate < distance_threshold)
		{
			current_position = vec4(origin + (ray * distance_travelled),
				1.f - (float(steps_taken) / float(max_steps)));
			break;
		}
	}

	// Write out absolute value of distance travelled:
	if (current_position.w < 0.01f) { out_distance = 0.f; }
	else { out_distance = abs(distance_travelled); }

	// Calculate normal based on distance gradient:
	out_normal = vec4(0.f, 1.f, 0.f, out_distance);

	// Return current position along with iterations achieved:
	return current_position;
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

float distance_estimator_cubes(vec3 position)
{
	vec4 z = vec4(position, 1.f);
	vec4 z0 = z;
	float min_radius = 0.25f;
	vec4 scale = vec4(0.28f, 0.28f, 0.28f, 0.28f) / min_radius;
	float scale_power = pow(0.28f, float(1-10));

	for (int i = 0; i < 9; i++)
	{
		z.xyz = clamp(z.xyz, -1.f, 1.f) * 2.f - z.xyz;

		float r2 = dot(z.xyz, z.xyz);
		z *= clamp(max(min_radius / r2, min_radius), 0.f, 1.f);

		z = z * scale + z0;
	}

	return ((length(z.xyz) - 0.18f) / z.w) - scale_power;
}
