#version 450

layout (location = 0) in vec3 iPosition;

layout (location = 0) out vec4 oColor;

void main()
{
	oColor = vec4(gl_FragCoord.xyz, 1.f);
}
