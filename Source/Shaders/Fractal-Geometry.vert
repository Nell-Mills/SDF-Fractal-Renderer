#version 450

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;

layout (location = 0) out vec3 oPosition;
layout (location = 1) out vec3 oNormal;

void main()
{
	oPosition = iPosition;
	oNormal = iNormal;

	gl_Position = vec4(iPosition, 1.f);
}
