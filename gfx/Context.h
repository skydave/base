#pragma once
#include <math/Math.h>
#include <util/shared_ptr.h>

#include "Attribute.h"
#include "Shader.h"
#include "Geometry.h"
#include "Camera.h"



namespace base
{
	BASE_DECL_SMARTPTR_STRUCT(Context);
	struct Context
	{

		Context();

		static ContextPtr                                                  current(); // returns current context


		// timing info
		float                                                                 time(); // returns current time
		void                                                   setTime( float time ); // sets current time

		// viewport info
		CameraPtr                                                           camera();
		void                                           setCamera( CameraPtr camera );


		//
		// global uniform manangement
		//
		void setUniform( const std::string &name, AttributePtr uniform );
		AttributePtr               getUniform( const std::string &name );
		std::map<std::string, AttributePtr> m_globalUniforms;


		//
		// rendering
		//
		void render( GeometryPtr geo, ShaderPtr shader );
		void            renderScreen( ShaderPtr shader );

		GeometryPtr                         m_screenQuad;

	private:
		// view =======================
		void setView( const math::Matrix44f &view, const math::Matrix44f &viewinv, const math::Matrix44f &proj );

		math::Matrix44f                                    m_modelMatrix; // object to world
		math::Matrix44f                              m_viewInverseMatrix; // world to camera
		math::Matrix44f                                     m_viewMatrix; // camera to world
		math::Matrix44f                               m_projectionMatrix; // camera to view


		AttributePtr                                          m_mvpmAttr; // model view projection matrix (world to screen)
		AttributePtr                                         m_vminvAttr; // view matrix inverse (camera to world)
		AttributePtr                                       m_mvminvtAttr; // model view matrix inverse transpose (model view matrix without scaling/shearing) used to transform vectors

		CameraPtr                                               m_camera;

		// time =========================
		float                                    m_time;
	};
}
