#include "Context.h"


#include <gfx/Context.h>


extern base::ContextPtr context;


namespace base
{
	namespace ops
	{

		std::stack<float>       Context::m_time;
		std::stack<CameraPtr> Context::m_camera;

		// pushes a new time onto the stack
		void Context::pushTime( float time )
		{
			m_time.push( time );
		}

		// removes time from the time stack
		float Context::popTime()
		{
			float t = time();
			m_time.pop();
			return t;
		}

		// returns current time, if stack is empty it will return base::Context::getUniform("time")
		float Context::time()
		{
			if( m_time.empty() )
				return context->getUniform("time")->get<float>(0);
			return m_time.top();
		}


		
		void Context::pushCamera( CameraPtr camera )
		{
			m_camera.push( camera );
			context->setView( camera->m_viewMatrix, camera->m_transform, camera->m_projectionMatrix );
		}

		CameraPtr Context::popCamera()
		{
			CameraPtr c = camera();
			m_camera.pop();
			CameraPtr current = camera();
			if( current )
				context->setView( current->m_viewMatrix, current->m_transform, current->m_projectionMatrix );
			return c;
		}

		CameraPtr Context::camera()
		{
			if( m_camera.empty() )
				return CameraPtr();
			return m_camera.top();
		}
	}
}
