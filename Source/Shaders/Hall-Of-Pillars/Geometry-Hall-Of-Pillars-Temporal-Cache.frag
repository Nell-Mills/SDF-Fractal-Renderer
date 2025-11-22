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
bool in_cube(vec3 cube_centre, float cube_size, vec3 point);

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
	float distance_travelled;
	float distance_threshold = 0.001f;

	// Step according to written distance:
	vec4 distance_sample = texture(u_distance_sampler, in_tex_coord).rgba;

	float distance_min = min(min(min(distance_sample.r, distance_sample.g),
					distance_sample.b), distance_sample.a);

	float distance_max = max(max(max(distance_sample.r, distance_sample.g),
					distance_sample.b), distance_sample.a);

	// Check for excess camera movement, scaled by how far away the point is:
	float invalidation_threshold = 0.0001f;
	if (abs(distance_max - distance_min) > (distance_min * invalidation_threshold))
	{ distance_travelled = 0.f; }
	else { distance_travelled = distance_min * 0.9f; }

	current_position = vec4(origin + (ray * distance_travelled), 1.f);

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
	out_distance = vec4(distance_travelled, distance_sample.rgb);

	// Return current position along with iterations achieved:
	return current_position;
}

float distance_estimator_hall_of_pillars(vec3 position)
{
        vec3 z = position.xzy;
        float scale = max(0.1f, u_scene.fractal_parameter - 1.f);
        vec3 size_clamp = vec3(1.f, 1.f, 1.3f);

        for (int i = 0; i < 12; i++)
        {
                z = (u_scene.fractal_parameter * clamp(z, -size_clamp, size_clamp)) - z;
                float r2 = dot(z, z);
                float k = max(u_scene.fractal_parameter / r2, 0.027f);
                z *= k;
                scale *= k;
        }

        float l = length(z.xy);
        float rxy = l - 4.f;
        float n = l * z.z;
        rxy = max(rxy, -n / 4.f);

        return rxy / abs(scale);
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
