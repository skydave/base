#include "Image.h"
#include <iostream>
#include <util/fs.h>
#include <util/stb_image.h>
#include <math/Math.h>






namespace base
{




	Image::Image() : m_data(0)
	{

	}

	Image::~Image()
	{
		if( m_data )
			free(m_data);
	}

	ImagePtr Image::copy( int x, int y, int width, int height )
	{
		ImagePtr result = Image::create( width, height );

		// copy scanlines
		for( int j=0;j<height;++j )
		{
			int i1 = (y+j)*m_width*4 + x*4;
			int i2 = j*width*4;
			memcpy( &(result->m_data[i2]), &(m_data[i1]), width*4 );
		}

		return result;
	}

	void Image::flip( bool vertical, bool horizontal )
	{
		ImagePtr temp = copy( 0, 0, m_width, m_height );

		if( vertical )
		{
			for( int j=0;j<m_height;++j )
			{
				int i1 = j*m_width*4;
				int i2 = (m_height - j - 1)*m_width*4;
				memcpy( &(m_data[i2]), &(temp->m_data[i1]), m_width*4 );
			}
		}

		if( horizontal )
		{
			unsigned char *temp2 = (unsigned char *)malloc( m_width*4*sizeof(unsigned char) );
			for( int j=0;j<m_height;++j )
			{
				int idx = j*m_width*4;
				memcpy( temp2, &m_data[idx], m_width*4 );
				for( int i=0;i<m_width;++i )
				{
					m_data[idx+i*4] = temp2[(m_width - i - 1)*4];
					m_data[idx+i*4+1] = temp2[(m_width - i - 1)*4+1];
					m_data[idx+i*4+2] = temp2[(m_width - i - 1)*4+2];
					m_data[idx+i*4+3] = temp2[(m_width - i - 1)*4+3];
				}

			}
			free(temp2);
		}
	}


	math::Color Image::lookup( float u, float v )
	{
		// nearest neighbour
		int x = int(u*m_width);
		int y = int(v*m_height);

		if( (x >= 0)&&(x<m_width)&&(y >= 0)&&(y<m_height) )
		{
			int pixel = y*m_width*4 + x*4;
			return math::setRGBColor( m_data[pixel], m_data[pixel+1], m_data[pixel+2], m_data[pixel+3] );
		}

		return math::Color::Black();
	}

	math::Color Image::lookup( int x, int y )
	{
		int pixel = y*m_width*4 + x*4;
		return math::setRGBColor( m_data[pixel], m_data[pixel+1], m_data[pixel+2], m_data[pixel+3] );
	}

	ImagePtr Image::create( int width, int height )
	{
		ImagePtr img = ImagePtr( new Image() );
		img->m_data = (unsigned char *)malloc( width*height*4*sizeof(char) );
		img->m_width = width;
		img->m_height = height;
		return img;
	}

	ImagePtr Image::load( const Path &file )
	{
		if( fs::exists( file ) )
		{
			fs::File *f = fs::open( file );

			// read file content
			unsigned int size = (unsigned int)fs::size(f);

			unsigned char *data = (unsigned char *)malloc( size*sizeof(unsigned char) );
			fs::read(f, data, size, sizeof(char));

			// close file
			fs::close(f);

			// load image from file content
			int width, height, comp;
			unsigned char *result = stbi_load_from_memory( data, size, &width, &height, &comp, 4 );

			// dispose file content
			free(data);

			if( result == NULL )
			{
				std::cout << "Image " << file.str() << " failed to load. " << stbi_failure_reason() << std::endl;
			} else
			{
				std::cout << "Image " << file.str() << " loaded.\n";

				ImagePtr img = ImagePtr( new Image() );
				img->m_data = result;
				img->m_width = width;
				img->m_height = height;
				return img;
			}

		}
		return ImagePtr();
	}

}
