#version 450

layout (location = 0) in vec2 in_tex_coord;

layout (set = 0, binding = 0) uniform UScene
{
	vec4 eye_position;
	mat4 camera;
} u_scene;

layout (set = 1, binding = 0) uniform sampler2D u_iterations_sampler;

layout (location = 0) out vec4 out_colour;

void main()
{
	// Define a colour palette:
	vec3 colour_palette[4] = {
		vec3(0.f, 0.f, 0.f),
		vec3(1.f, 0.f, 0.f),
		vec3(0.f, 1.f, 0.f),
		vec3(0.f, 0.f, 1.f)
	};

	float iterations_achieved = texture(u_iterations_sampler, in_tex_coord).r;
	iterations_achieved = 1.f - iterations_achieved;

	float coordinate = iterations_achieved * 3.f;

	vec3 lower_colour = colour_palette[int(coordinate)];
	vec3 higher_colour = colour_palette[int(coordinate) + 1];

	// Get distribution of the colours:
	float distribution = coordinate - floor(coordinate);

	out_colour = vec4(mix(lower_colour, higher_colour, distribution), 1.f);
}
