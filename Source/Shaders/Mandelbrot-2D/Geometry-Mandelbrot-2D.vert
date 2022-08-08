#version 450

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

layout (location = 0) out vec4 out_position;

void main()
{
	// Generate 2D coordinates for fullscreen triangle, in range -1 to 1:
	vec2 coord_2d = vec2(
		(gl_VertexIndex << 1) & 2,
		gl_VertexIndex & 2
	);
	coord_2d = coord_2d * 2.f - 1.f;

	out_position.x = coord_2d.x;
	out_position.y = coord_2d.y;
	out_position.z = max(u_scene.eye_position.z, 0.f);
	out_position.w = 1.f;

	out_position.xy /= max(out_position.z, 1.f);

	out_position.x -= u_scene.eye_position.x;
	out_position.y += u_scene.eye_position.y;

	// Scale x by aspect ratio:
	out_position.x *= u_scene.aspect_ratio;

	// Reverse y-coordinate:
	out_position.y *= -1.f;

	gl_Position = vec4(coord_2d, 0.f, 1.f);
}
