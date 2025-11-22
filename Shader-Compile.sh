# Create shader directories:
mkdir -p Assets/Shaders/Hall-Of-Pillars
mkdir -p Assets/Shaders/Mandelbrot-2D
mkdir -p Assets/Shaders/Mandelbulb

#################
# 2D Mandelbrot #
#################

echo "GLSLC: Compiling 2D Mandelbrot shaders..."

# Colour:
echo " ---> Colour-Mandelbrot-2D.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Mandelbrot-2D/Colour-Mandelbrot-2D.vert -o ./Assets/Shaders/Mandelbrot-2D/Colour-Mandelbrot-2D.vert.sprv
echo " ---> Colour-Mandelbrot-2D.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Mandelbrot-2D/Colour-Mandelbrot-2D.frag -o ./Assets/Shaders/Mandelbrot-2D/Colour-Mandelbrot-2D.frag.sprv

# Geometry:
echo " ---> Geometry-Mandelbrot-2D.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Mandelbrot-2D/Geometry-Mandelbrot-2D.vert -o ./Assets/Shaders/Mandelbrot-2D/Geometry-Mandelbrot-2D.vert.sprv
echo " ---> Geometry-Mandelbrot-2D.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Mandelbrot-2D/Geometry-Mandelbrot-2D.frag -o ./Assets/Shaders/Mandelbrot-2D/Geometry-Mandelbrot-2D.frag.sprv

echo "... done."

##############
# Mandelbulb #
##############

echo "GLSLC: Compiling Mandelbulb shaders..."

# Colour:
echo " ---> Colour-Mandelbulb.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Mandelbulb/Colour-Mandelbulb.vert -o ./Assets/Shaders/Mandelbulb/Colour-Mandelbulb.vert.sprv
echo " ---> Colour-Mandelbulb.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Mandelbulb/Colour-Mandelbulb.frag -o ./Assets/Shaders/Mandelbulb/Colour-Mandelbulb.frag.sprv

# Geometry, no SDF:
echo " ---> Geometry-Mandelbulb.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Mandelbulb/Geometry-Mandelbulb.vert -o ./Assets/Shaders/Mandelbulb/Geometry-Mandelbulb.vert.sprv
echo " ---> Geometry-Mandelbulb.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Mandelbulb/Geometry-Mandelbulb.frag -o ./Assets/Shaders/Mandelbulb/Geometry-Mandelbulb.frag.sprv

# Geometry, 3D SDF:
echo " ---> Geometry-Mandelbulb-SDF-3D.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Mandelbulb/Geometry-Mandelbulb-SDF-3D.vert -o ./Assets/Shaders/Mandelbulb/Geometry-Mandelbulb-SDF-3D.vert.sprv
echo " ---> Geometry-Mandelbulb-SDF-3D.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Mandelbulb/Geometry-Mandelbulb-SDF-3D.frag -o ./Assets/Shaders/Mandelbulb/Geometry-Mandelbulb-SDF-3D.frag.sprv

# Geometry, Temporal Cache:
echo " ---> Geometry-Mandelbulb-Temporal-Cache.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Mandelbulb/Geometry-Mandelbulb-Temporal-Cache.vert -o ./Assets/Shaders/Mandelbulb/Geometry-Mandelbulb-Temporal-Cache.vert.sprv
echo " ---> Geometry-Mandelbulb-Temporal-Cache.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Mandelbulb/Geometry-Mandelbulb-Temporal-Cache.frag -o ./Assets/Shaders/Mandelbulb/Geometry-Mandelbulb-Temporal-Cache.frag.sprv

echo "... done."

###################
# Hall-Of-Pillars #
###################

echo "GLSLC: Compiling Hall of Pillars shaders..."

# Colour:
echo " ---> Colour-Hall-Of-Pillars.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Hall-Of-Pillars/Colour-Hall-Of-Pillars.vert -o ./Assets/Shaders/Hall-Of-Pillars/Colour-Hall-Of-Pillars.vert.sprv
echo " ---> Colour-Hall-Of-Pillars.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Hall-Of-Pillars/Colour-Hall-Of-Pillars.frag -o ./Assets/Shaders/Hall-Of-Pillars/Colour-Hall-Of-Pillars.frag.sprv

# Geometry, no SDF:
echo " ---> Geometry-Hall-Of-Pillars.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars.vert -o ./Assets/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars.vert.sprv
echo " ---> Geometry-Hall-Of-Pillars.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars.frag -o ./Assets/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars.frag.sprv

# Geometry, 3D SDF:
echo " ---> Geometry-Hall-Of-Pillars-SDF-3D.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars-SDF-3D.vert -o ./Assets/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars-SDF-3D.vert.sprv
echo " ---> Geometry-Hall-Of-Pillars-SDF-3D.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars-SDF-3D.frag -o ./Assets/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars-SDF-3D.frag.sprv

# Geometry, Temporal Cache:
echo " ---> Geometry-Hall-Of-Pillars-Temporal-Cache.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars-Temporal-Cache.vert -o ./Assets/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars-Temporal-Cache.vert.sprv
echo " ---> Geometry-Hall-Of-Pillars-Temporal-Cache.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars-Temporal-Cache.frag -o ./Assets/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars-Temporal-Cache.frag.sprv

echo "... done."
