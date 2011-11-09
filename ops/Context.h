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
			static float                                    time(); // returns current time
			static void                      setTime( float time ); // sets current time

			// viewport info
			static CameraPtr                              camera();
			static void              setCamera( CameraPtr camera );




		private:
			static float                                    m_time;
			static CameraPtr                              m_camera;
		};
	}
}
