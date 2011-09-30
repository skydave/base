#pragma once


#include <stack>
#include <gfx/Camera.h>




namespace base
{
	namespace ops
	{
		//
		// This class holds cross Operator information
		//
		class Context
		{
		public:
			// timing info
			static void                    pushTime( float time ); // pushes a new time onto the stack
			static float                                popTime(); // removes time from the time stack
			static float                                   time(); // returns current time, if stack is empty it will return base::Context::getUniform("time")

			// viewport info
			static void            pushCamera( CameraPtr camera );
			static CameraPtr                          popCamera();
			static CameraPtr                             camera();




		private:
			static std::stack<float>                      m_time;
			static std::stack<CameraPtr>                m_camera;
		};
	}
}
