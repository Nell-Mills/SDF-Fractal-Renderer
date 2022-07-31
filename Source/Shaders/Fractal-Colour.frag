#version 450

layout (location = 0) in vec2 in_tex_coord;

layout (set = 0, binding = 0) uniform UScene
{
	vec3 plane_centre;
	vec3 x_axis;
	vec3 y_axis;
	vec3 eye_position;
	float mandelbulb_parameter;
} u_scene;

layout (set = 1, binding = 0) uniform sampler2D u_position_sampler;
layout (set = 2, binding = 0) uniform sampler2D u_normal_sampler;

layout (location = 0) out vec4 out_colour;

// Function prototypes:
vec4 colour_function(vec3 position);

void main()
{
	//float iterations_achieved = texture(u_position_sampler, in_tex_coord).a;
	//out_colour = vec4(vec3(iterations_achieved), 1.f);

	//float distance = texture(u_normal_sampler, in_tex_coord).a;
	//out_colour = vec4(vec3(distance), 1.f);

	vec3 position = texture(u_position_sampler, in_tex_coord).rgb;
	out_colour = colour_function(position);
}

vec4 colour_function(vec3 position)
{
	vec3 w = position;
	float m = dot(w, w);
	vec4 colour_parameters = vec4(abs(w), m);
	float dz = 1.f;

	for (int i = 0; i < 4; i++)
	{
		dz = (8.f * pow(sqrt(m), 7.f) * dz) + 1.f;
		float r = length(w);
		float b = u_scene.mandelbulb_parameter * acos(w.y / r);
		float a = u_scene.mandelbulb_parameter * atan(w.z, w.x);
		w = pow(r, 8.f) * vec3(sin(b) * sin(a), cos(b), sin(b) * cos(a)) + position;
		colour_parameters = min(colour_parameters, vec4(abs(w), m));
		m = dot(w, w);
		if (m > 256.f) { break; }
	}

	return vec4(m, colour_parameters.yzw);
}
