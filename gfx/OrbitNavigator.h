/*---------------------------------------------------------------------



----------------------------------------------------------------------*/
#pragma once
#include <math/Math.h>
#include "Camera.h"

namespace base
{

	///
	/// \brief is used to manipulate the camera from mouse movements and enables the user to control the camera like in a dcc-app
	///
	struct OrbitNavigator
	{
		OrbitNavigator();  // constructor
		OrbitNavigator( Camera *camera );  // constructor

		void                                                                   update();  ///< recomputes the transform from current polar coordinates and distance value

		void                           panView( float x, float y, bool update  = true );  ///< moves the camera on the view plane
		void orbitView( float azimuthDelta, float elevationDelta, bool update  = true );  ///< rotates the camera around the origin (angles in degree)
		void                       zoomView( float distanceDelta, bool update  = true );  ///< zoomes the camera in or out (depending on the sign of the amount parameter)
		void                setLookAt( float x, float y, float z, bool update  = true );  ///< sets the point to look at - change this to move the camera around
		void                    setLookAt( math::Vec3f newLookAt, bool update  = true );  ///< returns the distance of the camera to the lookat-point

		float                                                 getDistance( void ) const;


		Camera                                                                *m_camera;
		bool                                                                m_ownCamera;


		math::Vec3f                                                            m_lookAt;
		float                                                    m_azimuth, m_elevation;  // polar coordinates
		float                                                                m_distance;
	};


}
