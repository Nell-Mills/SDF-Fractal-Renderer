echo "GLSLC: Compiling Fractal-Geometry.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Geometry.vert -o ./Assets/Shaders/Fractal-Geometry.vert.sprv

echo "GLSLC: Compiling Fractal-Geometry.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Geometry.frag -o ./Assets/Shaders/Fractal-Geometry.frag.sprv

echo "GLSLC: Compiling Fractal-Geometry-2D-Mandelbrot.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Geometry-2D-Mandelbrot.vert -o ./Assets/Shaders/Fractal-Geometry-2D-Mandelbrot.vert.sprv

echo "GLSLC: Compiling Fractal-Geometry-2D-Mandelbrot.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Geometry-2D-Mandelbrot.frag -o ./Assets/Shaders/Fractal-Geometry-2D-Mandelbrot.frag.sprv

echo "GLSLC: Compiling Fractal-Geometry-SDF-2D.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Geometry-SDF-2D.vert -o ./Assets/Shaders/Fractal-Geometry-SDF-2D.vert.sprv

echo "GLSLC: Compiling Fractal-Geometry-SDF-2D.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Geometry-SDF-2D.frag -o ./Assets/Shaders/Fractal-Geometry-SDF-2D.frag.sprv

echo "GLSLC: Compiling Fractal-Geometry-SDF-3D.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Geometry-SDF-3D.frag -o ./Assets/Shaders/Fractal-Geometry-SDF-3D.frag.sprv

echo "GLSLC: Compiling Fractal-Colour.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Colour.vert -o ./Assets/Shaders/Fractal-Colour.vert.sprv

echo "GLSLC: Compiling Fractal-Colour.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Colour.frag -o ./Assets/Shaders/Fractal-Colour.frag.sprv
