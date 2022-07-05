# Compile the Fractal Renderer project.

# Wildcards:
DEPSVULKAN	:= $(wildcard ./Source/Vulkan/*.c)

CC		:= gcc
OUT		:= ./FracRender
MAIN		:= ./Source/Fractal-Renderer.c
DEPS		:= $(DEPSVULKAN)
THIRDPARTY	:= ./Third-Party/volk/src/volk.c
LFLAGS		:= -ldl -lglfw
SHADERS		:= ./Shader-Compile.sh

No-OpenMP: $(MAIN)
	$(CC) $(MAIN) $(DEPS) $(THIRDPARTY) -o $(OUT) $(LFLAGS) && $(SHADERS)

With-OpenMP: $(MAIN)
	$(CC) $(MAIN) $(DEPS) $(THIRDPARTY) -fopenmp -o $(OUT) $(LFLAGS) && $(SHADERS)

# Explanation of links:
# dl -> libdl, used for dlsym, dlopen, etc. Inside volk.c.
# glfw -> GLFW
