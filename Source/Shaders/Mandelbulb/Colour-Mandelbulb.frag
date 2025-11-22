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
vec4 colour_function_mandelbulb(vec3 position);

void main()
{
	vec4 position = texture(u_position_sampler, in_tex_coord).rgba;

	if ((position.w < 0.01f) || (length(position.xyz - u_scene.eye_position) >=
						(u_scene.view_distance * 0.9f)))
	{
		out_colour = vec4(1.f);
		return;
	}

	// Colour using colour function:
//	out_colour = colour_function_mandelbulb(position.xyz);

	// Colour using colour function, with iterations achieved for AO effect:
	out_colour = (colour_function_mandelbulb(position.xyz) * pow(position.w, 20));

	// Colour based on iterations achieved:
//	out_colour = vec4(vec3(pow(position.w, 20)), 1.f);
}

vec4 colour_function_mandelbulb(vec3 position)
{
        int max_iterations = 4;
        float escape_radius = 2.f;
        float parameter = u_scene.fractal_parameter;

        vec3 z = position;      // Z = Z^2 + C.
        float dr = 1.f;
        float r = 0.0;          // Radius.

	// Colour:
	vec3 reference_point = vec3(1.f, 0.75f, 0.5f);
	vec3 colour = abs(z - reference_point);
	vec3 colour_addition = vec3(0.05f, 0.1f, 0.15f);

        for (int i = 0; i < max_iterations; i++)
        {
                r = length(z);
                if (r > escape_radius) { break; }

                // Convert position to spherical coordinates:
                float theta = acos(z.z / r);
                float phi = atan(z.y, z.x);
                dr = (pow(r, parameter - 1.f) * parameter * dr) + 1.f;

                // Scale and rotate position:
                float zr = pow(r, parameter);
                theta *= parameter;
                phi *= parameter;

                // Convert position back to Cartesian coordinates:
                z = (zr * vec3(sin(theta) * cos(phi), sin(phi) * sin(theta),
                                                cos(theta))) + position;

		// Get colour (minimum distance from reference point):
		colour = min(colour, abs(z - reference_point));
        }

	return vec4(colour.zyx + colour_addition, 1.f);
}
