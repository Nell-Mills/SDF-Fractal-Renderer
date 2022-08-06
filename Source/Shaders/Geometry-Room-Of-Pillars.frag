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
float distance_estimator_room_of_pillars(vec3 position);

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

	for (int steps_taken = 0; steps_taken <= max_steps; steps_taken++)
	{
		// Get current position. Encode iterations in w-coordinate:
		current_position = vec4(origin + (ray * distance_travelled),
			1.f - (float(steps_taken) / float(max_steps)));

		// Get distance estimate and update total distance travelled:
		distance_estimate = distance_estimator_room_of_pillars(current_position.xyz);
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

float distance_estimator_room_of_pillars(vec3 position)
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
