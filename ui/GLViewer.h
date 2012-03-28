#pragma once

#include "Application.h"
#include "GLWindow.h"

#include <gfx/OrbitNavigator.h>

namespace base
{

	class GLViewer : public GLWindow
	{
	public:
		typedef void (*InitCallback)( void );
		typedef void (*RenderCallback)( CameraPtr );
		GLViewer( int width, int height, std::string caption, InitCallback init = 0, RenderCallback render = 0 );

		virtual void                                                                   paintGL();
		CameraPtr                                                                     getCamera();
		void        setView( math::Vec3f lookat, float distance, float azimuth, float elevation );


	private:
		OrbitNavigator m_orbitNavigator;
		InitCallback             m_init;
		RenderCallback         m_render;
	};
}
