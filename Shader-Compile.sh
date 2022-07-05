echo "GLSLC: Compiling Fractal-Geometry.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Geometry.vert -o ./Assets/Shaders/Fractal-Geometry.vert.sprv

echo "GLSLC: Compiling Fractal-Geometry.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Fractal-Geometry.frag -o ./Assets/Shaders/Fractal-Geometry.frag.sprv
