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

layout (set = 1, binding = 0) uniform sampler2D u_distance_sampler;

layout (location = 0) out vec4 out_position;
layout (location = 1) out vec4 out_normal;
layout (location = 2) out float out_distance;

// Main function:
void main()
{
	// Get ray from eye to pixel:
	vec3 ray = in_position.xyz - u_scene.eye_position;
	ray = normalize(ray);

	// Find closest point on Mandelbulb and calculate normal:
	out_position = vec4(1.f);
	out_normal = vec4(0.f, 1.f, 0.f, 1.f);
}
