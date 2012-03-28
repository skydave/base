//
// Purpose of GLTemporaryContext is to allow execution of gl functions when no gl context is available
// e.g. checking for available gl extensions which we only can do with a gl context
//

#pragma once

#include "Window.h"



namespace base
{
#ifdef _WINDOWS
	struct GLTemporaryContext
	{
		GLTemporaryContext();
		~GLTemporaryContext();

		HDC       dmy_pdc;
		HGLRC      dmy_rc;
		HDC        old_dc;
		HGLRC old_context;
		HWND       dmy_id;
	};
#endif
}