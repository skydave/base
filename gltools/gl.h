#pragma once


#define GLEW_STATIC
#include "glew.h"

#ifdef _WINDOWS
	#include <windows.h>
	#include <gl/gl.h>
#else
	#include "glxew.h"
	#include <GL/gl.h>
	#include <GL/glx.h>

	//
	// X11 pollutes namespace which clashes with qt headers
	// our workaround for this is to undef all defined names which cause trouble
	// and if needed define X11_<name> which is then used
	// This is bad. Here are potential alternatves:
	// 1. include qt headers before X11 headers
	//		this causes trouble with glew which is supposed to be included before opengl
	//		also the problem is still not solved
	// 2. compile qt with a custom defined namespace
	//		this would work but it would be very annoying to have to recompile qt with a
	//		custom namespace just to use base. It is important that it is easy to pick up.
	//
	//

	#ifdef Status
	#undef Status
	#endif
	#ifdef Bool
	#undef Bool
	#endif
	#ifdef CursorShape
	#undef CursorShape
	#endif
	#ifdef None
	#undef None
	#define X11_None 0L
	#endif
	#ifdef KeyPress
	#undef KeyPress
	#endif
	#ifdef KeyRelease
	#undef KeyRelease
	#endif
	#ifdef FocusIn
	#undef FocusIn
	#endif
	#ifdef FocusOut
	#undef FocusOut
	#endif
	#ifdef FontChange
	#undef FontChange
	#endif
#endif


