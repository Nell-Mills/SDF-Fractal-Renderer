#version 450

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec2 in_tex_coord;

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

	// View distance:
	float view_distance;
} u_scene;

layout (set = 1, binding = 0) uniform sampler2D u_distance_sampler;

layout (location = 0) out vec4 out_position;
layout (location = 1) out vec4 out_distance;

// Function prototypes:
vec4 sphere_trace(vec3 origin, vec3 ray);
float distance_estimator_hall_of_pillars(vec3 position);

// Main function:
void main()
{
	// Get ray from eye to pixel:
	vec3 ray = in_position.xyz - u_scene.eye_position;
	ray = normalize(ray);

	// Find closest point on Mandelbulb and calculate normal:
	out_position = sphere_trace(u_scene.eye_position, ray);
}

vec4 sphere_trace(vec3 origin, vec3 ray)
{
	vec4 current_position = vec4(origin, 1.f);
	int max_steps = 999;
	float distance_estimate;
	float distance_travelled = 0.f;
	float distance_threshold = 0.001f;

	// Step according to written distance:
	vec4 distance_sample = texture(u_distance_sampler, in_tex_coord).rgba;

	// See if the camera has moved too much:
	current_position = vec4(origin + (ray * distance_sample.r), 1.f);
	if (length(current_position.xyz - distance_sample.gba) >
		(min(length(current_position.xyz) * 0.5f, 0.005f)))
	{
		// Camera has moved too much, start ray at origin:
		current_position = vec4(origin, 1.f);
	}
	else
	{
		// Camera has not moved too much, can skip this:
		out_distance = vec4(distance_sample.r, distance_sample.gba);
		return current_position;
	}

	for (int steps_taken = 0; steps_taken <= max_steps; steps_taken++)
	{
		// Get distance estimate and update total distance travelled:
		distance_estimate = distance_estimator_hall_of_pillars(current_position.xyz);
		distance_travelled += distance_estimate;

		// Get current position. Encode iterations in w-coordinate:
		current_position = vec4(origin + (ray * distance_travelled),
			1.f - (float(steps_taken) / float(max_steps)));

		// Check how close the point is to the surface:
		if (distance_estimate < distance_threshold) { break; }

		// Check the view distance:
		if (abs(distance_travelled) >= u_scene.view_distance) { break; }
	}

	// Write out value of distance travelled:
	out_distance = vec4(distance_travelled, current_position.xyz);

	// Return current position along with iterations achieved:
	return current_position;
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
