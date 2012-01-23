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
		struct TransformState
		{

		private:
		};

		Context();

		static ContextPtr                                                                              current(); // returns current context
		static void                                                      setCurrentContext( ContextPtr context ); // sets current context


		// timing info
		float                                                                                             time(); // returns current time
		void                                                                               setTime( float time ); // sets current time

		// transform info
		CameraPtr                                                                                       camera();
		void                                                                       setCamera( CameraPtr camera );
		void setView( const math::Matrix44f &view, const math::Matrix44f &viewinv, const math::Matrix44f &proj ); // convinience function for updating transformState from view matrices

		void                                                setModelMatrix( const math::Matrix44f &modelMatrix );
		void                                                                                     setViewMatrix();
		void                                                                               setProjectionMatrix();

		void getTransformState( math::Matrix44f &modelMatrix, math::Matrix44f &viewMatrix, math::Matrix44f &projectionMatrix, math::Matrix44f &modelViewProjectionMatrix, math::Matrix44f &viewInverseMatrix, math::Matrix33f &modelViewInverseTranspose );
		void setTransformState( const math::Matrix44f &modelMatrix, const math::Matrix44f &viewMatrix, math::Matrix44f &projectionMatrix, const math::Matrix44f &modelViewProjectionMatrix, const math::Matrix44f &viewInverseMatrix, const math::Matrix33f &modelViewInverseTranspose );

		//
		// global uniform manangement
		//
		void                                         setUniform( const std::string &name, AttributePtr uniform );
		AttributePtr                                                       getUniform( const std::string &name );
		std::map<std::string, AttributePtr>                                                     m_globalUniforms;


		//
		// rendering
		//
		void render( GeometryPtr geo, ShaderPtr shader );
		void            renderScreen( ShaderPtr shader );

		void   bind( ShaderPtr shader, GeometryPtr geo = GeometryPtr() );
		void unbind( ShaderPtr shader, GeometryPtr geo = GeometryPtr() );

		GeometryPtr                         m_screenQuad;

	private:
		math::Matrix44f                                                                            m_modelMatrix; // object to world
		math::Matrix44f                                                                             m_viewMatrix; // world to camera
		math::Matrix44f                                                                       m_projectionMatrix; // camera to view
		math::Matrix44f                                                              m_modelViewProjectionMatrix; // model view projection matrix (world to screen)
		math::Matrix44f                                                                      m_viewInverseMatrix; // view matrix inverse (camera to world)
		math::Matrix33f                                                              m_modelViewInverseTranspose; // model view matrix inverse transpose (model view matrix without scaling/shearing) used to transform vectors




		AttributePtr                                                                                  m_mvpmAttr; // model view projection matrix (world to screen)
		AttributePtr                                                                                    m_mmAttr; // model matrix attribute
		AttributePtr                                                                                    m_vmAttr; // view matrix (camera to world) attribute
		AttributePtr                                                                                 m_vminvAttr; // view matrix inverse (camera to world)
		AttributePtr                                                                               m_mvminvtAttr; // model view matrix inverse transpose (model view matrix without scaling/shearing) used to transform vectors

		CameraPtr                                                                                       m_camera;

		// time =========================
		float                                                                                             m_time;

		// misc =========================
		static ContextPtr                                                                       m_currentContext;
	};
}
