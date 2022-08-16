# SDF-Fractal-Renderer
Rendering of 3D fractals, making use of either a Signed Distance Field or Temporal Caching.

# Operating System
Known to compile and run on Linux Mint 20.1. Written for Linux.

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

3. Whether to gradually change a select parameter over time for the fractal:  
	-1 --> Backwards animation (parameter decreasing).  
Default	 0 --> No animation.  
	 1 --> Forwards animation.  
	 2 --> Special saved animation. Choose animation in the code at the beginning of main().

4. Whether to take performance measurements:  
Default	-1 --> No measurements.  
	 0 --> Measure 100 frames, stop. Take median, etc... repeat.  
	 1 --> Measure every 1 frame, 100 times. Take median, etc. Meant for special animations.
