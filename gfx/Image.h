#pragma once

#include <util/shared_ptr.h>
#include <util/Path.h>






namespace base
{

	BASE_DECL_SMARTPTR(Image);
	struct Image
	{
		Image();

		int m_width;
		int m_height;
		unsigned char *m_data;


		static ImagePtr load( const Path &file );
	};





}




