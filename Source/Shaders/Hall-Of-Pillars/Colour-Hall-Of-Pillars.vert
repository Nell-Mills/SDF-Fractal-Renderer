#version 450

layout (location = 0) out vec2 out_tex_coord;

void main()
{
	// Get texture coordinates from fullscreen triangle:
	out_tex_coord = vec2(
		(gl_VertexIndex << 1) & 2,
		gl_VertexIndex & 2
	);

	// Get position from texture coordinates:
	gl_Position = vec4(
		out_tex_coord * 2.f - 1.f,
		0.f,
		1.f
	);
}
