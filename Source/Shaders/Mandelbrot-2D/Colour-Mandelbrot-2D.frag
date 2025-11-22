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

	// Special colouring for Mandelbrot 2D iterations:
	//if (position.w < 0.f) { out_colour = vec4(1.f, 0.f, 1.f, 1.f); return; }

	// Colour using colour function (Mandelbulb, from Chapter 33 of Ray Tracing Gems II):
	out_colour = colour_function_mandelbulb(vec3(position.w));

	// Colour based on iterations achieved:
	//out_colour = vec4(vec3(position.w), 1.f);
}

vec4 colour_function_mandelbulb(vec3 position)
{
	int max_iterations = 4;
        float escape_radius = 2.f;
        float parameter = 8.f;

        vec3 z = position;      // Z = Z^2 + C.
        float dr = 1.f;
        float r = 0.0;          // Radius.

	float m = dot(z, z);
	vec4 colour_parameters = vec4(abs(z), m);

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

		// Get colour:
		colour_parameters = min(colour_parameters, vec4(abs(z), m));

                // Convert position back to Cartesian coordinates:
                z = (zr * vec3(sin(theta) * cos(phi), sin(phi) * sin(theta),
                                                cos(theta))) + position;

		m = dot(z, z);
        }

	return vec4(m, colour_parameters.yzw);
}
