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

	// View distance:
	float view_distance;
} u_scene;

layout (set = 1, binding = 0) uniform sampler2D u_position_sampler;

layout (location = 0) out vec4 out_colour;

// Function prototypes:
vec4 colour_function_hall_of_pillars(vec3 position);

void main()
{
	vec4 position = texture(u_position_sampler, in_tex_coord).rgba;

	// Calculate fog:
	float fog = length(position.xyz - u_scene.eye_position) / u_scene.view_distance;
	fog = pow(fog, 10);

	// Colour using colour function:
//	out_colour = colour_function_hall_of_pillars(position.xyz) + fog;

	// Colour using colour function, with iterations achieved for AO effect:
	out_colour = (colour_function_hall_of_pillars(position.xyz) * pow(position.w, 10)) + fog;

	// Colour based on iterations achieved, with and without fog:
//	out_colour = vec4(vec3(pow(position.w, 10)), 1.f) + fog;
//	out_colour = vec4(vec3(pow(position.w, 10)), 1.f);
}

vec4 colour_function_hall_of_pillars(vec3 position)
{
	vec3 z = normalize(position.xzy);
	vec3 colour = z;
	float r2 = dot(z, z);
	vec3 size_clamp = vec3(1.f, 1.f, 1.3f);

	for (int i = 0; i < 5; i++)
	{
		vec3 z1 = (u_scene.fractal_parameter * clamp(z, -size_clamp, size_clamp)) - z;
		colour.x += abs(z.x - z1.z);
		colour.y += abs(z.y - z1.x);
		colour.z += abs(z.z + z1.y);
		z = z1;
		r2 = dot(z, z);
		float k = max(u_scene.fractal_parameter / r2, 0.027f);
		z *= k;
	}

	colour = normalize(colour);
	return vec4(colour, 1.f);
}
