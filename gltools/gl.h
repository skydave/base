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
#endif


