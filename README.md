# SDF-Fractal-Renderer
Rendering of 3D fractals, making use of either a Signed Distance Field or Temporal Caching.

# Operating System
Known to compile and run on Linux Mint 20.1. Written for Linux.

If you do want to try on other operating systems, the compilation includes Fractal-Renderer.c, as
well as everything in the Vulkan, SDF and Utility directories. Shaders will need to be compiled
as well, and placed in the appropriate directories. Use Shader-Compile.sh as a guide. You may need
to change the shader compilation program to suit your operating system.

# Libraries
Need to install GLFW 3:

sudo apt install libglfw3-dev

# Compilation
Run either:

Make debug  
Make release

Default is debug.

Debug build has Vulkan validation layers enabled.

# Arguments
1. Type of fractal.  
Default	-1 --> 2D Mandelbrot set.  
	 0 --> Mandelbulb.  
	 1 --> "Hall of Pillars".

2. Type of optimization.  
Default	-1 --> No optimization.  
	 0 --> 3D SDF.  
	 1 --> Temporal Caching.

3. Whether to animate the fractal:  
	-1 --> Backwards animation (parameter decreasing).  
Default	 0 --> No animation.  
	 1 --> Forwards animation (parameter increasing).  
	 2 --> Special saved animation.

4. Whether to take performance measurements:  
Default	-1 --> No measurements.  
	 0 --> Measure 100 frames, stop. Take median, etc... repeat.  
	 1 --> Measure every 1 frame, 100 times. Take median, etc. Meant for animations.
