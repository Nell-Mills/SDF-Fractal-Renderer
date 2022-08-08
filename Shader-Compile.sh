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

# Geometry, 2D SDF:
echo " ---> Geometry-Mandelbulb-SDF-2D.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Mandelbulb/Geometry-Mandelbulb-SDF-2D.vert -o ./Assets/Shaders/Mandelbulb/Geometry-Mandelbulb-SDF-2D.vert.sprv
echo " ---> Geometry-Mandelbulb-SDF-2D.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Mandelbulb/Geometry-Mandelbulb-SDF-2D.frag -o ./Assets/Shaders/Mandelbulb/Geometry-Mandelbulb-SDF-2D.frag.sprv

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

# Geometry, 2D SDF:
echo " ---> Geometry-Hall-Of-Pillars-SDF-2D.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars-SDF-2D.vert -o ./Assets/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars-SDF-2D.vert.sprv
echo " ---> Geometry-Hall-Of-Pillars-SDF-2D.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars-SDF-2D.frag -o ./Assets/Shaders/Hall-Of-Pillars/Geometry-Hall-Of-Pillars-SDF-2D.frag.sprv

echo "... done."

#######################
# Multiple Mandelbulb #
#######################

echo "GLSLC: Compiling Multiple Mandelbulb shaders..."

# Colour:
echo " ---> Colour-Multiple-Mandelbulb.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Multiple-Mandelbulb/Colour-Multiple-Mandelbulb.vert -o ./Assets/Shaders/Multiple-Mandelbulb/Colour-Multiple-Mandelbulb.vert.sprv
echo " ---> Colour-Multiple-Mandelbulb.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Multiple-Mandelbulb/Colour-Multiple-Mandelbulb.frag -o ./Assets/Shaders/Multiple-Mandelbulb/Colour-Multiple-Mandelbulb.frag.sprv

# Geometry, no SDF:
echo " ---> Geometry-Multiple-Mandelbulb.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Multiple-Mandelbulb/Geometry-Multiple-Mandelbulb.vert -o ./Assets/Shaders/Multiple-Mandelbulb/Geometry-Multiple-Mandelbulb.vert.sprv
echo " ---> Geometry-Multiple-Mandelbulb.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Multiple-Mandelbulb/Geometry-Multiple-Mandelbulb.frag -o ./Assets/Shaders/Multiple-Mandelbulb/Geometry-Multiple-Mandelbulb.frag.sprv

# Geometry, 3D SDF:
echo " ---> Geometry-Multiple-Mandelbulb-SDF-3D.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Multiple-Mandelbulb/Geometry-Multiple-Mandelbulb-SDF-3D.vert -o ./Assets/Shaders/Multiple-Mandelbulb/Geometry-Multiple-Mandelbulb-SDF-3D.vert.sprv
echo " ---> Geometry-Multiple-Mandelbulb-SDF-3D.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Multiple-Mandelbulb/Geometry-Multiple-Mandelbulb-SDF-3D.frag -o ./Assets/Shaders/Multiple-Mandelbulb/Geometry-Multiple-Mandelbulb-SDF-3D.frag.sprv

# Geometry, 2D SDF:
echo " ---> Geometry-Multiple-Mandelbulb-SDF-2D.vert"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Multiple-Mandelbulb/Geometry-Multiple-Mandelbulb-SDF-2D.vert -o ./Assets/Shaders/Multiple-Mandelbulb/Geometry-Multiple-Mandelbulb-SDF-2D.vert.sprv
echo " ---> Geometry-Multiple-Mandelbulb-SDF-2D.frag"
./Third-Party/glslc/linux-x86_64/glslc ./Source/Shaders/Multiple-Mandelbulb/Geometry-Multiple-Mandelbulb-SDF-2D.frag -o ./Assets/Shaders/Multiple-Mandelbulb/Geometry-Multiple-Mandelbulb-SDF-2D.frag.sprv

echo "... done."
