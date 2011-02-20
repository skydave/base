#include "Camera.h"

namespace base
{

	Camera::Camera()
	{
		m_znear = 0.1f;
		m_zfar  = 1000.0f;
		m_focalLength = 10.0;
		m_fov   = 45.0f;
		m_aspectRatio = 1.333f;
	}


	void Camera::update( void )
	{
		//calculate width and height at 1 unit in front of the camera
		float height = 2 * tanf( float(m_fov/2) );
		float width = height * m_aspectRatio;


		//create projection matrix
		float sc = ( m_zfar + m_znear ) / ( m_zfar - m_znear );
		float of = 2 * m_zfar * m_znear / ( m_zfar - m_znear );
		m_projectionMatrix.ma[ 0] = 2/width;
		m_projectionMatrix.ma[ 1] = 0;
		m_projectionMatrix.ma[ 2] = 0;
		m_projectionMatrix.ma[ 3] = 0;
		m_projectionMatrix.ma[ 4] = 0;
		m_projectionMatrix.ma[ 5] = 2/height;
		m_projectionMatrix.ma[ 6] = 0;
		m_projectionMatrix.ma[ 7] = 0;
		m_projectionMatrix.ma[ 8] = 0;
		m_projectionMatrix.ma[ 9] = 0;
		m_projectionMatrix.ma[10] = -sc;
		m_projectionMatrix.ma[11] = -1;
		m_projectionMatrix.ma[12] = 0;
		m_projectionMatrix.ma[13] = 0;
		m_projectionMatrix.ma[14] = -of;
		m_projectionMatrix.ma[15] = 0;

		// create view matrix
		m_viewMatrix = m_transform;
		// speedup inverse computation by transposing the rotational part of the matrix
		//m_viewMatrix.invert();
		math::Vec3f translation = m_viewMatrix.getTranslation();
		math::Matrix44f rotation = m_viewMatrix.getOrientation();
		rotation.transpose();
		m_viewMatrix = math::Matrix44f::TranslationMatrix( -translation ) * rotation;
	}
}