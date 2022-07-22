#version 450

layout (location = 0) in vec4 in_position;

layout (set = 0, binding = 0) uniform UScene
{
	vec3 eye_position;
	float aspect_ratio;
	mat4 camera;
} u_scene;

layout (location = 0) out vec4 out_iterations;

// 2D Mandelbrot function prototypes:
dvec2 square_complex(dvec2 complex_number);
float mandelbrot_2d(dvec4 pixel_location);

// 3D Mandelbrot (Mandelbulb) function prototypes:


// Main function:
void main()
{
	// Full view, 1:1 x:y ratio:
	double min_x = -1.8;
	double max_x = 0.7;
	double min_y = -1.25;
	double max_y = 1.25;

	// Zoomed in:
//	double min_x = -0.6636137233169267;
//	double max_x = -0.6393484547743432;
//	double min_y = -0.4555960782958095;
//	double max_y = -0.4313308097542259;

	// Really zoomed in:
//	double min_x = -0.48276924785372444;
//	double max_x = -0.4827692478531531;
//	double min_y = -0.6267432530084442;
//	double max_y = -0.6267432530078725;

	// Get axis dimensions:
	double dist_x = max_x - min_x;
	double dist_y = max_y - min_y;

	// Reposition the pixel according to above min/max values:
	dvec4 new_position;
	new_position.x = in_position.x;
	new_position.y = in_position.y;
	new_position.z = in_position.z;
	new_position.w = 1.0;

	// For x-coordinate, scale by aspect ratio:
	new_position.x *= (dist_x / 2.0) * u_scene.aspect_ratio;
	new_position.x += max_x - (dist_x / 2.0);

	// For y-coordinate, reverse direction to go -1 to 1, bottom-to-top:
	new_position.y *= -dist_y / 2.0;
	new_position.y += max_y - (dist_y / 2.0);

	// Calculate Mandelbrot iterations and output to texture image:
	float iterations_achieved = mandelbrot_2d(new_position);
	out_iterations = vec4(vec3(iterations_achieved), 1.f);
}

dvec2 square_complex(dvec2 complex_number)
{
	dvec2 result;
	result.x = (complex_number.x * complex_number.x) - (complex_number.y * complex_number.y);
	result.y = complex_number.x * complex_number.y * 2.0;

	return result;
}

float mandelbrot_2d(dvec4 pixel_location)
{
	// Equation: z = z^2 + c
	dvec2 z = dvec2(0.0, 0.0);
	dvec2 c = dvec2(pixel_location.x, pixel_location.y);

	int iterations_achieved = 0;
	int max_iterations = 500;
	double threshold_value = 2.0;

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
