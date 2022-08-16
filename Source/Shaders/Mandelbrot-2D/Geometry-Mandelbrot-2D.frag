#version 450

layout (location = 0) in vec4 in_position;

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

layout (location = 0) out vec4 out_position;

// 2D Mandelbrot function prototypes:
vec2 square_complex(vec2 complex_number);
float mandelbrot_2d(vec2 pixel_location);
bool is_iteration(vec2 pixel_location);

// Main function:
void main()
{
	// See if point is in set of first 25 iterations:
	//if (is_iteration(in_position.xy)) { out_iterations = vec4(-1.f); return; }

	// Calculate Mandelbrot iterations and output to texture image:
	float iterations_achieved = mandelbrot_2d(in_position.xy);
	out_position = vec4(in_position.xyz, iterations_achieved);
}

vec2 square_complex(vec2 complex_number)
{
	vec2 result;
	result.x = (complex_number.x * complex_number.x) - (complex_number.y * complex_number.y);
	result.y = complex_number.x * complex_number.y * 2.f;

	return result;
}

float mandelbrot_2d(vec2 pixel_location)
{
	float y_mult;
	if (pixel_location.y < 0.f) { y_mult = -1.f; }
	else { y_mult = 1.f; }

	// Equation: z = z^2 + c
	vec2 z = vec2(u_scene.fractal_parameter, u_scene.fractal_parameter * y_mult);
	vec2 c = pixel_location;

	int iterations_achieved = 0;
	int max_iterations = 250;
	float threshold_value = 2.f;

	for (int i = 0; i < max_iterations; i++)
	{
		z = square_complex(z) + c;
		if (length(z) > threshold_value) { break; }
		iterations_achieved++;
	}

	if (iterations_achieved == max_iterations) { return 1.f; }

	// Banded colouring:
	//return float(iterations_achieved) / float(max_iterations);

	// Smooth colouring (based on: https://linas.org/art-gallery/escape/escape.html):
	return (float(iterations_achieved) + 1.f - (log(log(length(z))) / log(2.f)))
							/ float(max_iterations);
}

bool is_iteration(vec2 pixel_location)
{
	// Set of points for first 25 iterations of Mandelbrot, starting from (0.3, 0.05):
	vec2 positions[] = {
		vec2( 0.300000f,  0.050000f),
		vec2( 0.387500f,  0.088750f),
		vec2( 0.442280f,  0.128505f),
		vec2( 0.479098f,  0.173133f),
		vec2( 0.499560f,  0.222980f),
		vec2( 0.499840f,  0.272909f),
		vec2( 0.475361f,  0.309460f),
		vec2( 0.430202f,  0.316261f),
		vec2( 0.385053f,  0.293554f),
		vec2( 0.362092f,  0.262587f),
		vec2( 0.362158f,  0.240196f),
		vec2( 0.373464f,  0.229409f),
		vec2( 0.386847f,  0.227493f),
		vec2( 0.397898f,  0.231038f),
		vec2( 0.404944f,  0.237115f),
		vec2( 0.407756f,  0.243370f),
		vec2( 0.407036f,  0.248121f),
		vec2( 0.404115f,  0.250539f),
		vec2( 0.400539f,  0.250701f),
		vec2( 0.397581f,  0.249348f),
		vec2( 0.395896f,  0.247432f),
		vec2( 0.395511f,  0.245724f),
		vec2( 0.396049f,  0.244637f),
		vec2( 0.397007f,  0.244246f),
		vec2( 0.397959f,  0.244400f)
	};

	for (int i = 0; i < 25; i++)
	{
		if ((in_position.x > (positions[i].x - 0.001f)) &&
			(in_position.x < (positions[i].x + 0.001f)) &&
			(in_position.y > (positions[i].y - 0.001f)) &&
			(in_position.y < (positions[i].y + 0.001f)))
		{
			return true;
		}
	}

	return false;
}
