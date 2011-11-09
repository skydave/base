#include "Context.h"


#include <gfx/Context.h>


extern base::ContextPtr context;


namespace base
{
	namespace ops
	{

		float          Context::m_time;
		CameraPtr    Context::m_camera;

		// sets time
		void Context::setTime( float time )
		{
			m_time = time;
		}

		// returns current time
		float Context::time()
		{
			return m_time;
		}


		
		void Context::setCamera( CameraPtr camera )
		{
			m_camera = camera;
			if(m_camera)
				context->setView( camera->m_viewMatrix, camera->m_transform, camera->m_projectionMatrix );
		}

		CameraPtr Context::camera()
		{
			return m_camera;
		}
	}
}
