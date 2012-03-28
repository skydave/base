#pragma once

#include "Application.h"
#include "GLWindow.h"

#include <gfx/OrbitNavigator.h>

namespace base
{

	class GLViewer : public GLWindow
	{
	public:
		typedef void (*RenderCallback)( CameraPtr );
		GLViewer();                                                                                // when using this constructor the window will be created when show is called first time
		GLViewer( int width, int height, std::string caption, GLWindow::InitCallback init = 0, RenderCallback render = 0 );

		virtual void                                                                    paintGL();
		CameraPtr                                                                     getCamera();
		void        setView( math::Vec3f lookat, float distance, float azimuth, float elevation );
		void                                           setRenderCallback( RenderCallback render );

	private:
		OrbitNavigator m_orbitNavigator;
		RenderCallback         m_render;
	};
}
