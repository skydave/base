#include "Image.h"
#include <iostream>
#include <util/fs.h>
#include <util/stb_image.h>






namespace base
{




	Image::Image()
	{

	}



	ImagePtr Image::load( const Path &file )
	{
		if( fs::exists( file ) )
		{
			fs::File *f = fs::open( file );
			std::cout << "FILE EXISTS!\n";

			// read file content
			unsigned int size = (unsigned int)fs::size(f);
			std::cout << "file size: " << size << std::endl;

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
				std::cout << "Image not loaded from memory " << stbi_failure_reason() << std::endl;
			} else
			{
				std::cout << "Image loaded from memory\n";

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
