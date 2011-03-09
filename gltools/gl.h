#pragma once



#ifdef _WINDOWS
	#include <windows.h>
	#define GLEW_STATIC
	#include "glew.h"
	#include <gl/gl.h>
#else
	#include <GL/gl.h>
#endif


