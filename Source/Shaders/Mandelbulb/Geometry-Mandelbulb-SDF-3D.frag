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

layout (set = 1, binding = 0) readonly buffer BVoxels
{
	float voxels[];
} b_voxels;

layout (location = 0) out vec4 out_position;

// Function prototypes:
vec4 sphere_trace(vec3 origin, vec3 ray);
uint sdf_3d_lookup(vec3 position);
bool in_cube(vec3 cube_centre, float cube_size, vec3 point);
float ray_cube(vec3 origin, vec3 ray);
float distance_estimator_mandelbulb(vec3 position);

// Main function:
void main()
{
	// Get ray from eye to pixel:
	vec3 ray = in_position.xyz - u_scene.eye_position;
	ray = normalize(ray);

	// Find closest point on Mandelbulb and calculate normal:
	out_position = sphere_trace(u_scene.eye_position, ray);
}

vec4 sphere_trace(vec3 origin, vec3 ray)
{
	vec4 current_position = vec4(origin, 1.f);
	int max_steps = 999;
	float distance_estimate;
	float distance_travelled = 0.f;
	float distance_threshold = 0.0001f;
	uint voxel_lookup;
	float cube_size = u_scene.sdf_3d_size / pow(2.f, u_scene.sdf_3d_levels);

	int steps_taken = 0;
	for (; steps_taken <= max_steps; steps_taken++)
	{
		// Look up which voxel the point is in:
		voxel_lookup = sdf_3d_lookup(current_position.xyz);

		// If voxel is invalid (0), check for main cube intersection:
		if (voxel_lookup == 0)
		{
			distance_estimate = ray_cube(origin, ray);
			if (distance_estimate >= 0.f)
			{
				distance_travelled += distance_estimate + distance_threshold;
			}
			else { break; }
		}
		else
		{
			distance_estimate = b_voxels.voxels[voxel_lookup];
			distance_travelled += distance_estimate;
		}

		current_position = vec4(origin + (ray * distance_travelled),
			1.f - (float(steps_taken) / float(max_steps)));

		if (abs(distance_estimate) <= (sqrt(3.f) * cube_size)) { break; }

		if (abs(distance_travelled) >= u_scene.view_distance) { break; }
	}

	for (; steps_taken <= max_steps; steps_taken++)
	{
		// Get distance estimate and update total distance travelled:
		distance_estimate = distance_estimator_mandelbulb(current_position.xyz);
		distance_travelled += distance_estimate;

		// Get current position. Encode iterations in w-coordinate:
		current_position = vec4(origin + (ray * distance_travelled),
			1.f - (float(steps_taken) / float(max_steps)));

		// Check how close the point is to the surface:
		if (distance_estimate < distance_threshold) { break; }

		// Check the view distance:
		if (abs(distance_travelled) >= u_scene.view_distance) { break; }
	}

	// Return current position along with iterations achieved:
	return current_position;
}

uint sdf_3d_lookup(vec3 position)
{
	// Ordering of sub-cubes (looking top-down, upper/lower is y-coordinate):
	vec3 sub_cubes[] = {
		vec3(-1.f,  1.f,  1.f),	// Upper, top-left.
		vec3( 1.f,  1.f,  1.f),	// Upper, top-right.
		vec3(-1.f,  1.f, -1.f),	// Upper, bottom-left.
		vec3( 1.f,  1.f, -1.f),	// Upper, bottom-right.
		vec3(-1.f, -1.f,  1.f),	// Lower, top-left.
		vec3( 1.f, -1.f,  1.f),	// Lower, top-right.
		vec3(-1.f, -1.f, -1.f),	// Lower, bottom-left.
		vec3( 1.f, -1.f, -1.f)	// Lower, bottom-right.
	};

	uint index = 0;
	vec3 centre = u_scene.sdf_3d_centre;
	float size = u_scene.sdf_3d_size;

	// Check if point is in main cube:
	if (!in_cube(centre, size, position)) { return 0; }

	// Find out which cube the point is in:
	uint level = 1;
	uint max_level = u_scene.sdf_3d_levels;
	while (true)
	{
		// Cut size in two:
		size /= 2.f;

		for (uint i = 0; i < 8; i++)
		{
			vec3 new_centre = centre + (sub_cubes[i] * size);
			if (in_cube(new_centre, size, position))
			{
				if (level == max_level) {
					return index + i;
				}

				// Move to sub-cubes:
				centre = new_centre;
				index += uint(pow(8.f, float(max_level) - float(level))) * i;
				break;
			}
		}

		if (level == max_level) { return 0; }
		level++;
	}
}

bool in_cube(vec3 cube_centre, float cube_size, vec3 point)
{
	if (	(point.x >= (cube_centre.x - cube_size)) &&
		(point.x <= (cube_centre.x + cube_size)) &&
		(point.y >= (cube_centre.y - cube_size)) &&
		(point.y <= (cube_centre.y + cube_size)) &&
		(point.z >= (cube_centre.z - cube_size)) &&
		(point.z <= (cube_centre.z + cube_size)))
	{
		return true;
	}

	return false;
}

float ray_cube(vec3 origin, vec3 ray)
{
	// Function is meant for cube centered on origin. Move ray origin to compensate:
	float cube_size = u_scene.sdf_3d_size;
	vec3 cube_centre = u_scene.sdf_3d_centre;
	vec3 new_origin = origin - cube_centre;

	// Function from: https://iquilezles.org/articles/intersectors/
	vec3 m = 1.f / ray;
	vec3 n = m * new_origin;
	vec3 k = abs(m) * vec3(cube_size);
	vec3 t1 = -n - k;
	vec3 t2 = -n + k;
	float tN = max(max(t1.x, t1.y), t1.z);
	float tF = min(min(t2.x, t2.y), t2.z);
	if ((tN > tF) || (tF < 0.f)) { return -1.f; }	// Doesn't intersect.
	return tN;	// Nearest intersection distance.
}

float distance_estimator_mandelbulb(vec3 position)
{
	int max_iterations = 4;
	float escape_radius = 2.f;
	float parameter = u_scene.fractal_parameter;

	vec3 z = position;	// Z = Z^2 + C.
	float dr = 1.f;
	float r = 0.0;		// Radius.

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
	}

	// Calculate distance:
	return 0.5f * log(r) * (r / dr);
}
