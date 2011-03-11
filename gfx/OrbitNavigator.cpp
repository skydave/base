/*---------------------------------------------------------------------



----------------------------------------------------------------------*/
#include "OrbitNavigator.h"
#include "Camera.h"

namespace base
{

	//
	// constructor
	//
	OrbitNavigator::OrbitNavigator()
	{
		// init local stuff
		m_lookAt = math::Vec3f( 0.0f, 0.0f, 0.0f );
		m_azimuth = m_elevation = 0.0f;
		m_distance = .5f;

		m_camera = CameraPtr(new Camera());

		update();
	}

	//
	// constructor
	//
	OrbitNavigator::OrbitNavigator( CameraPtr camera )
	{
		// init local stuff
		m_lookAt = math::Vec3f( 0.0f, 0.0f, 0.0f );
		m_azimuth = m_elevation = 0.0f;
		m_distance = .5f;

		m_camera = camera;

		update();
	}


	//
	// returns the m_distance to the m_lookAt point
	//
	float OrbitNavigator::getDistance( void ) const
	{
		return m_distance;
	}

	//
	//
	//
	void OrbitNavigator::update()
	{
		// compute the final m_transformation from m_lookAt and camera polar coordinates
		// and write the result to the writeOnlyReference
		math::Matrix44f m = math::Matrix44f::Identity();


		m.translate( math::Vec3f( 0.0f, 0.0f, m_distance ) );	

		//m.rotateZ( twist ); // not used
		m.rotateX( math::degToRad(m_elevation) );
		m.rotateY( math::degToRad(m_azimuth) );


		m.translate( m_lookAt );

		if( m_camera )
		{
			m_camera->m_transform = m;
			m_camera->update();
		}
	}

	//
	// moves the camera on the view plane
	//
	void OrbitNavigator::panView( float x, float y, bool _update )
	{
		m_lookAt += (-x*m_distance*0.01f)*m_camera->m_transform.getRight();
		m_lookAt += (-y*m_distance*0.01f)*m_camera->m_transform.getUp();
		if(_update)
			update();
	}

	//
	// rotates the camera around the origin
	//
	void OrbitNavigator::orbitView( float m_azimuthDelta, float m_elevationDelta, bool _update )
	{
		m_azimuth   += m_azimuthDelta;
		m_elevation += m_elevationDelta;
		if(_update)
			update();
	}

	//
	// zoomes the camera in or out (depending on the sign of the amount parameter)
	//
	void OrbitNavigator::zoomView( float m_distanceDelta, bool _update )
	{
		m_distance += m_distanceDelta;
		if(_update)
			update();
	}

	//
	//
	//
	void OrbitNavigator::setLookAt( float x, float y, float z, bool _update )
	{
		m_lookAt = math::Vec3f( x, y, z );
		if(_update)
			update();
	}

	//
	//
	//
	void OrbitNavigator::setLookAt( math::Vec3f newLookAt, bool _update )
	{
		m_lookAt = newLookAt;
		if(_update)
			update();
	}


}
