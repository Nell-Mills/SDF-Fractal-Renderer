#version 450

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;

layout (location = 0) out vec4 oColor;

void main()
{
	//oColor = vec4(iPosition, 1.f);
	oColor = vec4(iNormal, 1.f);
}
