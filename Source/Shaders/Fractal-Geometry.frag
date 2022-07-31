#version 450

layout (location = 0) in vec4 in_position;

layout (set = 0, binding = 0) uniform UScene
{
	vec3 plane_centre;
	vec3 x_axis;
	vec3 y_axis;
	vec3 eye_position;
	float mandelbulb_parameter;
} u_scene;

layout (location = 0) out vec4 out_position;
layout (location = 1) out vec4 out_normal;

// Function prototypes:
float distance_estimator(vec3 position);
vec4 raymarch(vec3 origin, vec3 ray);

// Main function:
void main()
{
	// Get ray from eye to pixel:
	vec3 ray = in_position.xyz - u_scene.eye_position;
	ray = normalize(ray);

	// Find closest point on Mandelbulb and calculate normal:
	out_position = raymarch(in_position.xyz, ray);
}

float distance_estimator(vec3 position)
{
	vec3 w = position;
	float m = dot(w, w);
	float dz = 1.f;

	for (int i = 0; i < 4; i++)
	{
		dz = (8.f * pow(sqrt(m), 7.f) * dz) + 1.f;
		float r = length(w);
		float b = u_scene.mandelbulb_parameter * acos(w.y / r);
		float a = u_scene.mandelbulb_parameter * atan(w.z, w.x);
		w = pow(r, 8) * vec3(sin(b) * sin(a), cos(b), sin(b) * cos(a)) + position;
		m = dot(w, w);
		if (m > 256.f) { break; }
	}

	return 0.25f * log(m) * sqrt(m) / dz;
}

vec4 raymarch(vec3 origin, vec3 ray)
{
	vec3 mandelbulb_positions[] = {
		vec3( 0.f, 0.f,  0.f),
		vec3(-1.f, 0.f,  1.f),
		vec3( 1.f, 0.f,  1.f),
		vec3(-1.f, 0.f, -1.f),
		vec3( 1.f, 0.f, -1.f)
	};

	vec4 current_position;
	int max_steps = 49;
	float distance_travelled = 0.f;
	float distance_threshold = 0.0001f;

	for (int steps_taken = 0; steps_taken <= max_steps; steps_taken++)
	{
		// Get current position. Encode iterations in w-coordinate:
		current_position = vec4(origin + (ray * distance_travelled),
			1.f - (float(steps_taken) / float(max_steps)));

		// Get distance estimate and update total distance travelled:
		float distance_estimates[5];
		for (int i = 0; i < 5; i++)
		{
			distance_estimates[i] = distance_estimator(current_position.xyz +
								mandelbulb_positions[i]);
		}

		float distance_estimate = min(min(min(min(distance_estimates[3],
				distance_estimates[4]), distance_estimates[2]),
				distance_estimates[1]), distance_estimates[0]);

		//float distance_estimate = distance_estimator(current_position.xyz);
		distance_travelled += distance_estimate;

		// Check how close the point is to the surface:
		if (distance_estimate < distance_threshold)
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
