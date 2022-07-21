#version 450

layout (location = 0) out vec4 out_position;

void main()
{
	// Generate 2D coordinates for fullscreen triangle:
	vec2 coord_2d = vec2(
		(gl_VertexIndex << 1) & 2,
		gl_VertexIndex & 2
	);

	// Get position in range -1 to 1:
	vec4 position = vec4(
		coord_2d * 2.f - 1.f,
		0.f,
		1.f
	);

	// Pass to fragment shader:
	out_position = position;
	gl_Position = position;
}
