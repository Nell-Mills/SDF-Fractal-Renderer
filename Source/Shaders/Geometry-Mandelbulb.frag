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

layout (location = 0) out vec4 out_position;

// Function prototypes:
vec4 raymarch(vec3 origin, vec3 ray);
float distance_estimator_mandelbulb(vec3 position);

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
	float distance_travelled = 0.f;
	float distance_threshold = 0.0001f;

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
		distance_estimate = distance_estimator_mandelbulb(current_position.xyz);

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

float distance_estimator_mandelbulb(vec3 position)
{
	int max_iterations = 4;
	float escape_radius = 2.f;
	float parameter = u_scene.fractal_parameter;

	vec3 z = position;	// Z = Z^2 + C.
	float dr = 1.f;
	float r = 0.0;		// Radius.

	for (int i = 0; i < max_iterations; i++)
	{
		r = length(z);
		if (r > escape_radius) { break; }

		// Convert position to spherical coordinates:
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
