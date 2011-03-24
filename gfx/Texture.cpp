#include "Texture.h"

#include <gltools/gl.h>
//#include "Attribute.h"

namespace base
{

	//
	// Texture1d ---------------------------------
	//


	Texture1dPtr Texture1d::create( int textureFormat, int xres )
	{
		Texture1dPtr result = Texture1dPtr(new Texture1d());

		result->m_xres = xres;
		result->m_textureFormat = textureFormat;

		glBindTexture(GL_TEXTURE_1D, result->m_id);
		glTexImage1D(GL_TEXTURE_1D, 0, result->m_textureFormat, result->m_xres, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		return result;
	}

	Texture1dPtr Texture1d::createRGBA8( int xres )
	{
		return Texture1d::create( GL_RGBA8, xres );
	}

	Texture1dPtr Texture1d::createRGBAFloat32( int xres )
	{
		return Texture1d::create( GL_RGBA_FLOAT32_ATI, xres );
	}

	Texture1dPtr Texture1d::createFloat32( int xres )
	{
		/*
		Texture1dPtr result = Texture1dPtr(new Texture1d());

		result->m_textureFormat = GL_RGBA8;

		glBindTexture(GL_TEXTURE_2D, result->m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, result->m_textureFormat,  m_xres, m_yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		return result;
		*/
		return Texture1dPtr();
	}


	Texture1d::Texture1d()
	{
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_1D, m_id);

		// when texture area is small, bilinear filter the closest mipmap
		glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		//glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		// when texture area is large, bilinear filter the original
		glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		//glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		// the texture wraps over at the edges (repeat)
		glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	}

	Texture1d::~Texture1d()
	{
		glDeleteTextures(1,&m_id);
	}


	void Texture1d::uploadRGBA8( int xres, unsigned char *pixels )
	{
		m_xres = xres;
		glBindTexture(GL_TEXTURE_1D, m_id);
		glTexImage1D(GL_TEXTURE_1D, 0, m_textureFormat, m_xres, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}

	AttributePtr Texture1d::getUniform()
	{
		if( !m_uniform )
		{
			m_uniform = AttributePtr( new Sampler1dAttribute() );
			m_uniform->appendElement( (int)m_id );
		}
		return m_uniform;
	}






	//
	// Texture2d ---------------------------------
	//






	Texture2dPtr Texture2d::create( int textureFormat, int xres, int yres )
	{
		Texture2dPtr result = Texture2dPtr(new Texture2d());

		result->m_xres = xres;
		result->m_yres = yres;
		result->m_textureFormat = textureFormat;

		glBindTexture(GL_TEXTURE_2D, result->m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, result->m_textureFormat, result->m_xres, result->m_yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		return result;
	}

	Texture2dPtr Texture2d::createRGBA8( int xres, int yres )
	{
		return Texture2d::create( GL_RGBA8, xres, yres );
	}

	Texture2dPtr Texture2d::createRGBAFloat32( int xres, int yres )
	{
		return Texture2d::create( GL_RGBA_FLOAT32_ATI, xres, yres );
	}

	Texture2dPtr Texture2d::createFloat32( int xres, int yres )
	{
		/*
		Texture2dPtr result = Texture2dPtr(new Texture2d());

		result->m_textureFormat = GL_RGBA8;

		glBindTexture(GL_TEXTURE_2D, result->m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, result->m_textureFormat,  m_xres, m_yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		return result;
		*/
		return Texture2dPtr();
	}


	Texture2d::Texture2d()
	{
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		// when texture area is small, bilinear filter the closest mipmap
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		// when texture area is large, bilinear filter the original
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		// the texture wraps over at the edges (repeat)
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	}

	Texture2d::~Texture2d()
	{
		glDeleteTextures(1,&m_id);
	}


	void Texture2d::uploadRGBA8( int xres, int yres, unsigned char *pixels )
	{
		m_xres = xres;
		m_yres = yres;
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, m_textureFormat, m_xres, m_yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}

	AttributePtr Texture2d::getUniform()
	{
		if( !m_uniform )
		{
			m_uniform = AttributePtr( new Sampler2dAttribute() );
			m_uniform->appendElement( (int)m_id );
		}
		return m_uniform;
	}






	//
	// Texture3d ---------------------------------
	//



	Texture3dPtr Texture3d::create( int textureFormat, int xres, int yres, int zres )
	{
		Texture3dPtr result = Texture3dPtr(new Texture3d());

		result->m_xres = xres;
		result->m_yres = yres;
		result->m_zres = zres;
		result->m_textureFormat = textureFormat;

		glBindTexture(GL_TEXTURE_3D_EXT, result->m_id);
		glTexImage3DEXT(GL_TEXTURE_3D_EXT, 0, result->m_textureFormat, result->m_xres, result->m_yres, result->m_zres, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		return result;
	}

	Texture3dPtr Texture3d::createRGBA8( int xres, int yres, int zres )
	{
		return Texture3d::create( GL_RGBA8, xres, yres, zres );
	}

	Texture3dPtr Texture3d::createRGBAFloat32( int xres, int yres, int zres )
	{
		return Texture3d::create( GL_RGBA_FLOAT32_ATI, xres, yres, zres );
	}

	Texture3dPtr Texture3d::createFloat32( int xres, int yres, int zres )
	{
		/*
		Texture2dPtr result = Texture2dPtr(new Texture2d());

		result->m_textureFormat = GL_RGBA8;

		glBindTexture(GL_TEXTURE_2D, result->m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, result->m_textureFormat,  m_xres, m_yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		return result;
		*/
		return Texture3dPtr();
	}


	Texture3d::Texture3d()
	{
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_3D_EXT, m_id);

		// when texture area is small, bilinear filter the closest mipmap
		glTexParameterf( GL_TEXTURE_3D_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		// when texture area is large, bilinear filter the original
		glTexParameterf( GL_TEXTURE_3D_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		// the texture wraps over at the edges (repeat)
		glTexParameterf( GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameterf( GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP );
		glTexParameterf( GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_R, GL_CLAMP );
	}

	Texture3d::~Texture3d()
	{
		glDeleteTextures(1,&m_id);
	}


	void Texture3d::uploadRGBA8( int xres, int yres, int zres, unsigned char *pixels )
	{
		m_xres = xres;
		m_yres = yres;
		m_zres = zres;
		glBindTexture(GL_TEXTURE_3D_EXT, m_id);
		glTexImage3DEXT(GL_TEXTURE_3D_EXT, 0, m_textureFormat, m_xres, m_yres, m_zres, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}

	AttributePtr Texture3d::getUniform()
	{
		if( !m_uniform )
		{
			m_uniform = AttributePtr( new Sampler3dAttribute() );
			m_uniform->appendElement( (int)m_id );
		}
		return m_uniform;
	}
}
