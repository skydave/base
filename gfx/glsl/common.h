#pragma once
#include <util/shared_ptr.h>
#include <gfx/Texture.h>

extern char common[];
extern int common_size;


namespace base
{
	namespace glsl
	{
		extern Texture2dPtr g_noisePermutationTableTex;
		Texture2dPtr noisePermutationTableTex();
		std::string common();
	}
}
