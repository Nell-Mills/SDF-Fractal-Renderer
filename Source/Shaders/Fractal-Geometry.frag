#version 450

layout (location = 0) in vec4 in_position;

layout (set = 0, binding = 0) uniform UScene
{
	vec4 eye_position;
	mat4 camera;
} u_scene;

layout (location = 0) out vec4 out_iterations;

// 2D Mandelbrot functions:
vec2 square_complex(vec2 complex_number);
float mandelbrot_2d(vec4 pixel_location);

// 3D Mandelbrot (Mandelbulb) functions:

void main()
{
	// Position the pixel to get a better view of the 2D Mandelbrot set:
	vec4 new_position = in_position * 1.5;
	new_position.x -= 0.5f;

	float iterations_achieved = mandelbrot_2d(new_position);

	out_iterations = vec4(vec3(iterations_achieved), 1.f);
}

vec2 square_complex(vec2 complex_number)
{
	vec2 result;
	result.x = (complex_number.x * complex_number.x) - (complex_number.y * complex_number.y);
	result.y = complex_number.x * complex_number.y * 2.f;

	return result;
}

float mandelbrot_2d(vec4 pixel_location)
{
	// Equation: z = z^2 + c
	vec2 z = vec2(0.f, 0.f);
	vec2 c = vec2(pixel_location.x, pixel_location.y);

	int iterations_achieved = 0;
	int max_iterations = 100;
	float threshold_value = 2.f;

	for (int i = 0; i < max_iterations; i++)
	{
		z = square_complex(z) + c;

		if (length(z) > threshold_value)
		{
			break;
		}

		iterations_achieved++;
	}

	return float(iterations_achieved) / float(max_iterations);
}
