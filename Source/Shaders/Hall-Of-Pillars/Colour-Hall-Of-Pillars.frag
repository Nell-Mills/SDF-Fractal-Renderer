#version 450

layout (location = 0) in vec2 in_tex_coord;

layout (set = 0, binding = 0) uniform UScene
{
	// Axes in eye coordinate system:
	vec3 plane_centre;
	vec3 x_axis;
	vec3 y_axis;

	// Eye position:
	vec3 eye_position;

	// 3D SDF information:
	vec3 sdf_3d_centre;
	float sdf_3d_size;
	uint sdf_3d_levels;

	// Aspect ratio:
	float aspect_ratio;

	// Fractal parameter:
	float fractal_parameter;
} u_scene;

layout (set = 1, binding = 0) uniform sampler2D u_position_sampler;

layout (location = 0) out vec4 out_colour;

// Function prototypes:
vec4 colour_function_hall_of_pillars(vec3 position);

void main()
{
	// Colour using colour function:
/*	vec3 position = texture(u_position_sampler, in_tex_coord).rgb;
	out_colour = colour_function_hall_of_pillars(position);*/

	// Colour based on iterations achieved:
	float iterations = texture(u_position_sampler, in_tex_coord).a;
	out_colour = vec4(vec3(iterations), 1.f);
}

vec4 colour_function_hall_of_pillars(vec3 position)
{
	vec3 z = position.xzy;
	float col = 0.f;
	float r2 = dot(z, z);
	vec3 size_clamp = vec3(1.f, 1.f, 1.3f);

	for (int i = 0; i < 5; i++)
	{
		vec3 z1 = (2.f * clamp(z, -size_clamp, size_clamp)) - z;
		col += abs(z.x - z1.z);
		z = z1;
		r2 = dot(z, z);
		float k = max(2.f / r2, 0.027f);
		z *= k;
	}

	z = clamp(z + 1.f, vec3(0.f), vec3(1.f));

	return vec4(z.x * 0.1f * col, z.y * 0.5f * col, z.z * col, 1.f);
}
