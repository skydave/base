#pragma once


#include <stack>
#include <gfx/Camera.h>
#include <gfx/Context.h>




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
			static base::ContextPtr                      context();
			static     void setContext( base::ContextPtr context );

			// timing info
			static float                                    time(); // returns current time
			static void                      setTime( float time ); // sets current time

			// viewport info
			static CameraPtr                              camera();
			static void              setCamera( CameraPtr camera );




		private:
			static float                                    m_time;
			static CameraPtr                              m_camera;
			static base::ContextPtr                      m_context; // I dont like the idea of merging the low level gfx stuff with the ops - so there we are
		};
	}
}
