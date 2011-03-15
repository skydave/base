#include "Texture2d.h"

#include <gltools/gl.h>
//#include "Attribute.h"

namespace base
{
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

	Texture2dPtr createRGBAFloat32( int xres, int yres )
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


	void Texture2d::uploadRGBA8( int xres, int yres, char *pixels )
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

/*
	Texture2d::Texture2d( int xres, int yres, int textureFormat, int pixelFormat, int componentType, void *pixels  ) : m_uniform(0), m_xres(yres), m_yres(xres), m_textureFormat(textureFormat), m_pixelFormat(pixelFormat), m_componentType(componentType)
	{
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexImage2D(GL_TEXTURE_2D, 0, m_textureFormat,  m_xres, m_yres, 0, m_pixelFormat, m_componentType, pixels);

		// when texture area is small, bilinear filter the closest mipmap
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		// when texture area is large, bilinear filter the original
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		// the texture wraps over at the edges (repeat)
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

		printf( "texture created - texid: %i\n", m_id );
	}
*/

/*
	Texture::Texture( int xres, int yres, int textureFormat, int pixelFormat, int componentType, void *pixels  ) : m_uniform(0), m_xres(yres), m_yres(xres), m_textureFormat(textureFormat), m_pixelFormat(pixelFormat), m_componentType(componentType)
	{
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  xres, yres, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexImage2D(GL_TEXTURE_2D, 0, m_textureFormat,  m_xres, m_yres, 0, m_pixelFormat, m_componentType, pixels);

		// when texture area is small, bilinear filter the closest mipmap
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		// when texture area is large, bilinear filter the original
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		// the texture wraps over at the edges (repeat)
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

		printf( "texture created - texid: %i\n", m_id );
	}


	Attribute *Texture::getUniform()
	{
		if( !m_uniform )
		{
			m_uniform = new Attribute( 1, sizeof(int) );
			m_uniform->m_componentType = ATTR_TYPE_SAMPLER;
			m_uniform->appendElement( (int)m_id );
		}
		return m_uniform;
	}







	//
	// will return a texture with its own unique texture id
	//
	Texture *Texture::copy()
	{
		Texture *cpy = new Texture( m_xres, m_yres, m_textureFormat, m_pixelFormat, m_componentType, 0 );
		// TODO: cpy contents?
		return cpy;
	}


	void Texture::setSize( int newXres, int newYres )
	{
		if( (m_xres != newXres)||(m_yres != newYres) )
		{
			glBindTexture(GL_TEXTURE_2D, m_id);
			glTexImage2D(GL_TEXTURE_2D, 0, m_textureFormat, newXres, newYres, 0, m_pixelFormat, m_componentType, 0);

			m_xres = newXres;
			m_yres = newYres;
		}
	}








	Texture *Texture::createRGBA8( int xres, int yres, unsigned char *pixels )
	{
		return new Texture( xres, yres, GL_RGBA8,  GL_RGBA, GL_UNSIGNED_BYTE, pixels );
	}



*/
}
