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

layout (location = 0) out vec4 out_iterations;

// Function prototypes:
float mandelbulb(vec3 position);
float distance_estimator(vec3 position);
vec4 raymarch(vec3 origin, vec3 ray);

// Main function:
void main()
{
	// Get position on Mandelbulb surface:
	vec3 ray = in_position.xyz - u_scene.eye_position;
	ray = normalize(ray);
	vec4 surface = raymarch(in_position.xyz, ray);

	// Calculate Mandelbulb iterations and output to texture image:
//	float iterations_achieved = mandelbulb(surface);
//	out_iterations = vec4(vec3(iterations_achieved), 1.f);

	out_iterations = surface;
}

float mandelbulb(vec3 pixel_location)
{
	int iterations_achieved = 0;
	int max_iterations = 500;
	float threshold_value = 2.f;

	return float(iterations_achieved) / float(max_iterations);
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
		float b = 8.f * acos(w.y / r);
		float a = 8.f * atan(w.z, w.x);
		w = pow(r, 8) * vec3(sin(b) * sin(a), cos(b), sin(b) * cos(a)) + position;
		m = dot(w, w);
		if (m > 256.f) { break; }
	}

	return 0.25f * log(m) * sqrt(m) / dz;
}

vec4 raymarch(vec3 origin, vec3 ray)
{
	vec4 current_position;
	int max_steps = 50;
	float distance_travelled = 0.f;
	float distance_threshold = 0.0001f;

	for (int steps_taken = 0; steps_taken < max_steps; steps_taken++)
	{
		// Get current position:
		current_position = vec4(origin + (ray * distance_travelled),
					1.f - (float(steps_taken) / float(max_steps)));

		// Get distance estimate and update total distance travelled:
		float distance_estimate = distance_estimator(current_position.xyz);
		distance_travelled += distance_estimate * 0.9f;

		// Check how close the point is to the surface:
		if (distance_estimate < distance_threshold)
		{
			break;
		}
	}

	return current_position;
}
