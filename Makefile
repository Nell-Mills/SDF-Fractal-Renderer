# Compile the Fractal Renderer project.

# Wildcards:
DEPSVULKAN	:= $(wildcard ./Source/Vulkan/*.c)
DEPSUTILITY	:= $(wildcard ./Source/Utility/*.c)
DEPSSDF		:= $(wildcard ./Source/SDF/*.c)

CC		:= gcc
OUT		:= ./FracRender
MAIN		:= ./Source/Fractal-Renderer.c
DEPS		:= $(DEPSVULKAN) $(DEPSUTILITY) $(DEPSSDF)
THIRDPARTY	:= ./Third-Party/volk/src/volk.c
LFLAGS		:= -ldl -lglfw -lm
SHADERS		:= ./Shader-Compile.sh
DEBUG		:= -DFRACRENDER_DEBUG -g

release: $(MAIN)
	$(CC) $(MAIN) $(DEPS) $(THIRDPARTY) -o $(OUT) $(LFLAGS) && $(SHADERS)

debug: $(MAIN)
	$(CC) $(MAIN) $(DEBUG) $(DEPS) $(THIRDPARTY) -o $(OUT) $(LFLAGS) && $(SHADERS)

# Explanation of links:
# dl -> libdl, used for dlsym, dlopen, etc. Inside volk.c.
# glfw -> GLFW
# m -> Maths library
