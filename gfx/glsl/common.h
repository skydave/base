#pragma once
#include <util/shared_ptr.h>
#include <gfx/Texture.h>


namespace base
{
	namespace glsl
	{
		extern Texture2dPtr g_noisePermutationTableTex;
		Texture2dPtr noisePermutationTableTex();
	}
}
