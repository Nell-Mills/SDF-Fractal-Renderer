#version 450

layout (location = 0) in vec2 in_tex_coord;

layout (set = 0, binding = 0) uniform UScene
{
	vec3 plane_centre;
	vec3 x_axis;
	vec3 y_axis;
	vec3 eye_position;
	float aspect_ratio;
} u_scene;

layout (set = 1, binding = 0) uniform sampler2D u_position_sampler;
layout (set = 2, binding = 0) uniform sampler2D u_normal_sampler;

layout (location = 0) out vec4 out_colour;

void main()
{
	float iterations_achieved = texture(u_position_sampler, in_tex_coord).a;

	out_colour = vec4(vec3(iterations_achieved), 1.f);
	//out_colour = texture(u_position_sampler, in_tex_coord).rgba;
}
