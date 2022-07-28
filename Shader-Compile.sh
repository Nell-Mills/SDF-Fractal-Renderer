echo "GLSLC: Compiling Fractal-Geometry.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Geometry.vert -o ./Assets/Shaders/Fractal-Geometry.vert.sprv

echo "GLSLC: Compiling Fractal-Geometry.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Geometry.frag -o ./Assets/Shaders/Fractal-Geometry.frag.sprv

echo "GLSLC: Compiling Fractal-Geometry-SDF.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Geometry-SDF.frag -o ./Assets/Shaders/Fractal-Geometry-SDF.frag.sprv

echo "GLSLC: Compiling Fractal-Colour.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Colour.vert -o ./Assets/Shaders/Fractal-Colour.vert.sprv

echo "GLSLC: Compiling Fractal-Colour.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Colour.frag -o ./Assets/Shaders/Fractal-Colour.frag.sprv
