#version 450

layout (location = 0) in vec4 in_position;

layout (set = 0, binding = 0) uniform UScene
{
	vec3 eye_position;
	float aspect_ratio;
	mat4 camera;
} u_scene;

layout (location = 0) out vec4 out_iterations;

// Function prototypes:
float mandelbulb(vec3 position);
float distance_estimator(vec3 position);
vec3 raymarch(vec3 origin, vec3 ray);

// Main function:
void main()
{
	// Full view, 1:1 x:y ratio:
	float min_x = -1.f;
	float max_x =  1.f;
	float min_y = -1.f;
	float max_y =  1.f;

	// Get axis dimensions:
	float dist_x = max_x - min_x;
	float dist_y = max_y - min_y;

	// Reposition the pixel according to above min/max values:
	vec3 new_position;
	new_position.x = in_position.x;
	new_position.y = in_position.y;
	new_position.z = in_position.z;

	// For x-coordinate, scale by aspect ratio:
	new_position.x *= (dist_x / 2.f) * u_scene.aspect_ratio;
	new_position.x += max_x - (dist_x / 2.f);

	// For y-coordinate, reverse direction to go -1 to 1, bottom-to-top:
	new_position.y *= -dist_y / 2.f;
	new_position.y += max_y - (dist_y / 2.f);

	// Transform by camera matrix:
	new_position = vec3(u_scene.camera * vec4(new_position, 1.f));

	// Get position on Mandelbulb surface:
	vec3 ray = new_position - u_scene.eye_position;
	vec3 surface = raymarch(new_position, ray);

	// Calculate Mandelbulb iterations and output to texture image:
//	float iterations_achieved = mandelbulb(surface);
//	out_iterations = vec4(vec3(iterations_achieved), 1.f);

	out_iterations = vec4(surface, 1.f);
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

vec3 raymarch(vec3 origin, vec3 ray)
{
	vec3 current_position;
	int max_steps = 25;
	float distance_travelled = 0.f;
	float distance_threshold = 0.0001f;

	for (int steps_taken = 0; steps_taken < max_steps; steps_taken++)
	{
		// Get current position:
		current_position = origin + (ray * distance_travelled);

		// Get distance estimate and update total distance travelled:
		float distance_estimate = distance_estimator(current_position);
		distance_travelled += distance_estimate;

		// Check how close the point is to the surface:
		if (distance_estimate < distance_threshold)
		{
			break;
		}
	}

	return current_position;
}
