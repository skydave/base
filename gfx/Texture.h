#pragma once

#include <util/shared_ptr.h>
#include "Attribute.h"
#include "Image.h"

namespace base
{
	BASE_DECL_SMARTPTR(Texture1d);

	struct Texture1d
	{
		Texture1d();
		~Texture1d();

		void          uploadRGBA8( int xres, unsigned char *pixels );
		void            uploadRGBAFloat32( int xres, float *pixels );
		void                uploadFloat32( int xres, float *pixels );
		AttributePtr                                    getUniform();


		unsigned int                                            m_id; // opengl handle
		AttributePtr                                       m_uniform; // will be used to attach textures to geometry/or shaders which then will be bound
		int										              m_xres; // texture info just for the record
		int                                          m_textureFormat; // format of texture in gpu memory

		static Texture1dPtr create( int textureFormat, int xres = 64 );
		static Texture1dPtr createRGBA8( int xres = 64 );
		static Texture1dPtr createFloat32( int xres = 64 );
		static Texture1dPtr createRGBAFloat32( int xres = 64 );
	};

	BASE_DECL_SMARTPTR(Texture2d);

	struct Texture2d
	{
		Texture2d();
		~Texture2d();

		void uploadRGBA8( int xres, int yres, unsigned char *pixels );
		void   uploadRGBAFloat32( int xres, int yres, float *pixels );
		void       uploadFloat32( int xres, int yres, float *pixels );
		void                                 upload( ImagePtr image );
		AttributePtr                                     getUniform();


		unsigned int                                             m_id; // opengl handle
		AttributePtr                                        m_uniform; // will be used to attach textures to geometry/or shaders which then will be bound
		int                                            m_xres, m_yres; // texture info just for the record
		int                                           m_textureFormat; // format of texture in gpu memory

		static Texture2dPtr create( int textureFormat, int xres = 64, int yres = 64 );
		static Texture2dPtr createRGBA8( int xres = 64, int yres = 64 );
		static Texture2dPtr createFloat32( int xres = 64, int yres = 64 );
		static Texture2dPtr createRGBAFloat32( int xres = 64, int yres = 64 );
	};





	BASE_DECL_SMARTPTR(Texture3d);

	struct Texture3d
	{
		Texture3d();
		~Texture3d();

		void              uploadRGBA8( int xres, int yres, int zres, unsigned char *pixels );
		void                uploadRGBAFloat32( int xres, int yres, int zres, float *pixels );
		//void                    uploadFloat32( int xres, int yres, int zres, float *pixels );
		AttributePtr                                                            getUniform();


		unsigned int                                                                    m_id; // opengl handle
		AttributePtr                                                               m_uniform; // will be used to attach textures to geometry/or shaders which then will be bound
		int                                                           m_xres, m_yres, m_zres; // texture info just for the record
		int                                                                  m_textureFormat; // format of texture in gpu memory

		static Texture3dPtr create( int textureFormat, int xres = 64, int yres = 64, int zres = 64 );
		static Texture3dPtr createRGBA8( int xres = 64, int yres = 64, int zres = 64 );
		static Texture3dPtr createFloat32( int xres = 64, int yres = 64, int zres = 64 );
		static Texture3dPtr createRGBAFloat32( int xres = 64, int yres = 64, int zres = 64 );
	};


}
