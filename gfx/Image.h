#pragma once

#include <util/shared_ptr.h>
#include <util/Path.h>







namespace base
{

	BASE_DECL_SMARTPTR(Image);
	struct Image
	{
		Image();



		static ImagePtr load( const Path &file );
	};





}




