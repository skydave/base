#pragma once

#include <util/shared_ptr.h>
#include <util/Path.h>
#include <math/Color.h>






namespace base
{

	BASE_DECL_SMARTPTR_STRUCT(Image);
	struct Image
	{
		Image();
		~Image();

		int                                                              m_width;
		int                                                             m_height;
		unsigned char                                                    *m_data;

		math::Color                                   lookup( float u, float v );

		ImagePtr                     copy( int x, int y, int width, int height );
		void                flip( bool vertical = true, bool horizontal = true );


		static ImagePtr                          create( int width, int height );
		static ImagePtr                                 load( const Path &file );
	};





}




