#include "noise.h"
#include <string>
#include <math/Math.h>
#include <gltools/gl.h>
#include <util/fs.h>
#include <iostream>
#include <fstream>
#include <sstream>
namespace base
{
	namespace glsl
	{

		Texture2dPtr noisePermutationTableTex()
		{
			static Texture2dPtr g_noisePermutationTableTex = Texture2dPtr();
			if( !g_noisePermutationTableTex )
			{
				// generate noise permutation texture ===========
				unsigned char *pixels = (unsigned  char*)malloc( 256*256*4 );
				for(int i = 0; i<256; i++)
					for(int j = 0; j<256; j++)
					{
						int offset = (i*256+j)*4;
						unsigned char value = (int)math::perlinnoise::perm[(j+(int)math::perlinnoise::perm[i]) & 0xFF];
						pixels[offset] = (int)math::perlinnoise::grad3[value & 0x0F][0] * 64 + 64;   // Gradient x
						pixels[offset+1] = (int)math::perlinnoise::grad3[value & 0x0F][1] * 64 + 64; // Gradient y
						pixels[offset+2] = (int)math::perlinnoise::grad3[value & 0x0F][2] * 64 + 64; // Gradient z
						pixels[offset+3] = int((value & 0xFF)*0.5);                     // Permuted index
					}

				g_noisePermutationTableTex = base::Texture2d::createRGBA8();
				g_noisePermutationTableTex->uploadRGBA8(256, 256, pixels);
				// set wrap mode
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

				free( pixels );
			}

			return g_noisePermutationTableTex;
		}

		std::string noiseSrc()
		{
			// load from disk
			Path srcFile = base::Path( BASE_PATH ) + "/gfx/glsl/noise.glsl";
			return fs::read(srcFile);
			/*
			// TODO: use filesystem to abstract away fileaccess or alternatively bake noise code into cpp
			std::string src;
			std::ifstream file(srcFile.c_str() );
			if (file.is_open())
			{
				std::stringstream buffer;
				buffer << file.rdbuf();
				src = buffer.str();
				file.close();
			}
			return src;
			*/
		}
	}
}
