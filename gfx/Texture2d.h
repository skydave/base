#pragma once

#include <util/shared_ptr.h>
#include "Attribute.h"

namespace base
{

	BASE_DECL_SMARTPTR(Texture2d);

	struct Texture2d
	{
		Texture2d();
		~Texture2d();

		void         uploadRGBA8( int xres, int yres, char *pixels );
		AttributePtr                                    getUniform();


		unsigned int                                            m_id; // opengl handle
		AttributePtr                                       m_uniform; // will be used to attach textures to geometry/or shaders which then will be bound
		int                                           m_xres, m_yres; // texture info just for the record
		int                                          m_textureFormat; // format of texture in gpu memory

		/*
		Texture( int xres, int yres, int textureFormat = GL_RGBA_FLOAT32_ATI, int pixelFormat=GL_RGBA, int componentType = GL_FLOAT, void *pixels = NULL );


		Attribute                                         *m_uniform; // will be used to attach textures to geometry/or shaders which then will be bound
		int          m_textureFormat, m_pixelFormat, m_componentType; //

		void setSize( int newXres, int newYres );
		Texture *copy(); // will return a texture with its own unique texture id


		*/
		static Texture2dPtr create( int textureFormat, int xres = 64, int yres = 64 );
		static Texture2dPtr createRGBA8( int xres = 64, int yres = 64 );
		static Texture2dPtr createFloat32( int xres = 64, int yres = 64 );
		static Texture2dPtr createRGBAFloat32( int xres = 64, int yres = 64 );
	};


}
