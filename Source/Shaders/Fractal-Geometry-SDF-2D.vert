#version 450

layout (set = 0, binding = 0) uniform UScene
{
	vec3 plane_centre;
	vec3 x_axis;
	vec3 y_axis;
	vec3 eye_position;
	float mandelbulb_parameter;
} u_scene;

layout (location = 0) out vec4 out_position;
layout (location = 1) out vec2 out_tex_coord;

void main()
{
	// Generate 2D coordinates for fullscreen triangle, in range -1 to 1:
	vec2 coord_2d = vec2(
		(gl_VertexIndex << 1) & 2,
		gl_VertexIndex & 2
	);
	out_tex_coord = coord_2d;
	coord_2d = (coord_2d * 2.f) - 1.f;

	// Get position according to current plane transform:
	vec3 position = u_scene.plane_centre;

	// Translate along x-axis:
	position += u_scene.x_axis * coord_2d.x;

	// Translate along y-axis:
	position += u_scene.y_axis * coord_2d.y;

	// Pass to fragment shader:
	out_position = vec4(position, 1.f);
	gl_Position = vec4(coord_2d, 0.f, 1.f);
}