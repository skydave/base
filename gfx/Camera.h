#pragma once
#include <util/shared_ptr.h>
#include <math/Math.h>

namespace base
{
	BASE_DECL_SMARTPTR_STRUCT(Camera);

	struct Camera
	{
		Camera();


		virtual                             void update( void ); ///< will compute the cameras projection and view matricees


		float                                           m_znear;
		float                                     m_focalLength; // used for dof
		float                                            m_zfar;
		float                                             m_fov;
		float                                     m_aspectRatio;

		math::Matrix44f                      m_projectionMatrix;
		math::Matrix44f                            m_viewMatrix; ///< the matrix which transforms from world to camera space - the inverse of this matrix is the transform of the camera
		math::Matrix44f                     m_inverseViewMatrix;
		math::Matrix44f                             m_transform; ///< the matrix which transforms the camera from local into world space - the inverse of this matrix is the view transform
	};

}
