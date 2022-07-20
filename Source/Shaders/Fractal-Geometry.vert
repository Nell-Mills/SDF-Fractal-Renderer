#version 450

layout (location = 0) out vec3 oPosition;

void main()
{
	vec2 oTexCoord = vec2(
		(gl_VertexIndex << 1) & 2,
		gl_VertexIndex & 2
	);

	oPosition = vec3(
		oTexCoord * 2.f - 1.f,
		0.f
	);

	gl_Position = vec4(oPosition, 1.f);
}
