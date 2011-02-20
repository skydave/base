#pragma once

#include "Application.h"
#include "GLWindow.h"

#include <gfx/OrbitNavigator.h>

#ifdef _WINDOWS

namespace base
{

	class GLViewer : public GLWindow
	{
	public:
		GLViewer( int width, int height, std::string caption );

		virtual void paintGL();


	private:
		OrbitNavigator m_orbitNavigator;
	};
}
#endif

#ifdef linux

#endif

