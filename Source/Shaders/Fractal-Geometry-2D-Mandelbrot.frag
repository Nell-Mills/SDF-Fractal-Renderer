#version 450

layout (location = 0) in vec4 in_position;

layout (set = 0, binding = 0) uniform UScene
{
	vec3 plane_centre;
	vec3 x_axis;
	vec3 y_axis;
	vec3 eye_position;
	float mandelbulb_parameter;
} u_scene;

layout (location = 0) out vec4 out_iterations;

// 2D Mandelbrot function prototypes:
vec2 square_complex(vec2 complex_number);
float mandelbrot_2d(vec4 pixel_location);
bool is_iteration(vec2 pixel_location);

// Main function:
void main()
{
	// See if point is in set of first 100 iterations:
/*	if (is_iteration(in_position.xy))
	{
		out_iterations = vec4(-1.f);
		return;
	}*/

	// Calculate Mandelbrot iterations and output to texture image:
	float iterations_achieved = mandelbrot_2d(in_position);
	out_iterations = vec4(iterations_achieved);
}

vec2 square_complex(vec2 complex_number)
{
	vec2 result;
	result.x = (complex_number.x * complex_number.x) - (complex_number.y * complex_number.y);
	result.y = complex_number.x * complex_number.y * 2.0;

	return result;
}

float mandelbrot_2d(vec4 pixel_location)
{
	// Equation: z = z^2 + c
	vec2 z = vec2(0.0, 0.0);
	vec2 c = vec2(pixel_location.x, pixel_location.y);

	int iterations_achieved = 0;
	int max_iterations = 500;
	float threshold_value = 2.0;

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

bool is_iteration(vec2 pixel_location)
{
	// Set of points for first 100 iterations of Mandelbrot, starting from (0.3, 0.047):
	vec2 positions[] = {
		vec2( 0.300000f,  0.047000f),
		vec2( 0.387791f,  0.083452f),
		vec2( 0.443418f,  0.121008f),
		vec2( 0.481976f,  0.163646f),
		vec2( 0.505521f,  0.212453f),
		vec2( 0.510415f,  0.263879f),
		vec2( 0.490891f,  0.306072f),
		vec2( 0.447295f,  0.320808f),
		vec2( 0.397155f,  0.301821f),
		vec2( 0.366636f,  0.268317f),
		vec2( 0.362428f,  0.241491f),
		vec2( 0.373036f,  0.227170f),
		vec2( 0.387550f,  0.223079f),
		vec2( 0.400431f,  0.225655f),
		vec2( 0.409424f,  0.231778f),
		vec2( 0.413907f,  0.238869f),
		vec2( 0.414261f,  0.244908f),
		vec2( 0.411632f,  0.248624f),
		vec2( 0.407627f,  0.249692f),
		vec2( 0.403814f,  0.248658f),
		vec2( 0.401235f,  0.246541f),
		vec2( 0.400207f,  0.244335f),
		vec2( 0.400466f,  0.242696f),
		vec2( 0.401472f,  0.241871f),
		vec2( 0.402678f,  0.241792f),
		vec2( 0.403686f,  0.242217f),
		vec2( 0.404294f,  0.242853f),
		vec2( 0.404476f,  0.243457f),
		vec2( 0.404330f,  0.243873f),
		vec2( 0.404008f,  0.244054f),
		vec2( 0.403660f,  0.244030f),
		vec2( 0.403391f,  0.243879f),
		vec2( 0.403248f,  0.243687f),
		vec2( 0.403225f,  0.243522f),
		vec2( 0.403288f,  0.243419f),
		vec2( 0.403388f,  0.243385f),
		vec2( 0.403486f,  0.243405f),
		vec2( 0.403555f,  0.243454f),
		vec2( 0.403587f,  0.243510f),
		vec2( 0.403585f,  0.243554f),
		vec2( 0.403562f,  0.243579f),
		vec2( 0.403532f,  0.243584f),
		vec2( 0.403505f,  0.243574f),
		vec2( 0.403488f,  0.243559f),
		vec2( 0.403482f,  0.243543f),
		vec2( 0.403484f,  0.243532f),
		vec2( 0.403492f,  0.243526f),
		vec2( 0.403501f,  0.243526f),
		vec2( 0.403508f,  0.243529f),
		vec2( 0.403512f,  0.243534f),
		vec2( 0.403513f,  0.243538f),
		vec2( 0.403512f,  0.243541f),
		vec2( 0.403509f,  0.243543f),
		vec2( 0.403507f,  0.243542f),
		vec2( 0.403505f,  0.243541f),
		vec2( 0.403504f,  0.243540f),
		vec2( 0.403504f,  0.243538f),
		vec2( 0.403505f,  0.243538f),
		vec2( 0.403505f,  0.243538f),
		vec2( 0.403506f,  0.243538f),
		vec2( 0.403506f,  0.243538f),
		vec2( 0.403507f,  0.243538f),
		vec2( 0.403507f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f),
		vec2( 0.403506f,  0.243539f)
	};

	for (int i = 0; i < 100; i++)
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
