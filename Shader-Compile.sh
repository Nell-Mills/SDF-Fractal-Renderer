# Colour pass:
echo "GLSLC: Compiling Colour.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Colour.vert -o ./Assets/Shaders/Colour.vert.sprv

echo "GLSLC: Compiling Colour.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Colour.frag -o ./Assets/Shaders/Colour.frag.sprv

# Geometry pass, 2D Mandelbrot:
echo "GLSLC: Compiling Geometry-Mandelbrot-2D.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Geometry-Mandelbrot-2D.vert -o ./Assets/Shaders/Geometry-Mandelbrot-2D.vert.sprv

echo "GLSLC: Compiling Geometry-Mandelbrot-2D.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Geometry-Mandelbrot-2D.frag -o ./Assets/Shaders/Geometry-Mandelbrot-2D.frag.sprv

# Geometry pass, Mandelbulb, no SDF:
echo "GLSLC: Compiling Geometry-Mandelbulb.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Geometry-Mandelbulb.vert -o ./Assets/Shaders/Geometry-Mandelbulb.vert.sprv

echo "GLSLC: Compiling Geometry-Mandelbulb.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Geometry-Mandelbulb.frag -o ./Assets/Shaders/Geometry-Mandelbulb.frag.sprv

# Geometry pass, Mandelbulb, 3D SDF:
echo "GLSLC: Compiling Geometry-Mandelbulb-SDF-3D.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Geometry-Mandelbulb-SDF-3D.vert -o ./Assets/Shaders/Geometry-Mandelbulb-SDF-3D.vert.sprv

echo "GLSLC: Compiling Geometry-Mandelbulb-SDF-3D.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Geometry-Mandelbulb-SDF-3D.frag -o ./Assets/Shaders/Geometry-Mandelbulb-SDF-3D.frag.sprv

# Geometry pass, Mandelbulb, 2D SDF:
echo "GLSLC: Compiling Geometry-Mandelbulb-SDF-2D.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Geometry-Mandelbulb-SDF-2D.vert -o ./Assets/Shaders/Geometry-Mandelbulb-SDF-2D.vert.sprv

echo "GLSLC: Compiling Geometry-Mandelbulb-SDF-2D.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Geometry-Mandelbulb-SDF-2D.frag -o ./Assets/Shaders/Geometry-Mandelbulb-SDF-2D.frag.sprv

# Geometry pass, Room of Pillars, no SDF:
echo "GLSLC: Compiling Geometry-Room-Of-Pillars.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Geometry-Room-Of-Pillars.vert -o ./Assets/Shaders/Geometry-Room-Of-Pillars.vert.sprv

echo "GLSLC: Compiling Geometry-Room-Of-Pillars.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Geometry-Room-Of-Pillars.frag -o ./Assets/Shaders/Geometry-Room-Of-Pillars.frag.sprv

# Geometry pass, Room of Pillars, 3D SDF:
echo "GLSLC: Compiling Geometry-Room-Of-Pillars-SDF-3D.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Geometry-Room-Of-Pillars-SDF-3D.vert -o ./Assets/Shaders/Geometry-Room-Of-Pillars-SDF-3D.vert.sprv

echo "GLSLC: Compiling Geometry-Room-Of-Pillars-SDF-3D.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Geometry-Room-Of-Pillars-SDF-3D.frag -o ./Assets/Shaders/Geometry-Room-Of-Pillars-SDF-3D.frag.sprv

# Geometry pass, Room of Pillars, 2D SDF:
echo "GLSLC: Compiling Geometry-Room-Of-Pillars-SDF-2D.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Geometry-Room-Of-Pillars-SDF-2D.vert -o ./Assets/Shaders/Geometry-Room-Of-Pillars-SDF-2D.vert.sprv

echo "GLSLC: Compiling Geometry-Room-Of-Pillars-SDF-2D.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Geometry-Room-Of-Pillars-SDF-2D.frag -o ./Assets/Shaders/Geometry-Room-Of-Pillars-SDF-2D.frag.sprv
