#include "Image.h"
#include <iostream>
#include <util/fs.h>






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
			unsigned long size = fs::size(f);
			std::cout << "file size: " << size << std::endl;

			unsigned char *data = (unsigned char *)malloc( size*sizeof(unsigned char) );
			fs::read(f, data, size, sizeof(char));

			// TODO:load image from file content

			// dispose file content
			free(data);


			fs::close(f);
		}
		return ImagePtr();
	}

}
