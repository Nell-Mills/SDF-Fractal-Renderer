#ifndef FRACRENDER_VULKAN_CONFIG_H
#define FRACRENDER_VULKAN_CONFIG_H

#define FRACRENDER_DEBUG

#ifdef FRACRENDER_DEBUG
	#define ENABLE_VALIDATION 0
#else
	#define ENABLE_VALIDATION -1
#endif

#endif
