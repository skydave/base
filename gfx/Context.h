#pragma once
#include <math/Math.h>
#include <util/shared_ptr.h>

#include "Attribute.h"
#include "Shader.h"
#include "Geometry.h"
#include "Camera.h"
#include "Texture.h"



namespace base
{
	BASE_DECL_SMARTPTR_STRUCT(Context);
	struct Context
	{
		// used to store and cache all matrices (including redundant ones) of the transform pipeline
		struct TransformState
		{
			math::Matrix44f                                                                          modelMatrix; // object to world
			math::Matrix44f                                                                           viewMatrix; // world to camera
			math::Matrix44f                                                                      modelViewMatrix; // world to eye
			math::Matrix44f                                                                     projectionMatrix; // camera to view
			math::Matrix44f                                                            modelViewProjectionMatrix; // model view projection matrix (world to screen)
			math::Matrix44f                                                                    viewInverseMatrix; // view matrix inverse (camera to world)
			math::Matrix44f                                                                     modelViewInverse; // model view matrix inverse
			math::Matrix33f                                                            modelViewInverseTranspose; // model view matrix inverse transpose (model view matrix without scaling/shearing) used to transform vectors
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
		math::Vec3f                                                   worldToView( const math::Vec3f &worldPos );

		math::Matrix44f                                                                    getModelViewInverse();
		math::Matrix44f                                                                         getViewInverse();

		void                                                       getTransformState( TransformState &ts ) const;
		void                                                       setTransformState( const TransformState &ts );

		//
		// global uniform manangement
		//
		void                                         setUniform( const std::string &name, AttributePtr uniform );
		AttributePtr                                                       getUniform( const std::string &name );
		std::map<std::string, AttributePtr>                                                     m_globalUniforms;


		//
		// rendering
		//
		void                                                         render( GeometryPtr geo, ShaderPtr shader );
		void                           render( GeometryPtr geo, ShaderPtr shader, const math::Matrix44f &xform );
		void                                                                    renderScreen( ShaderPtr shader );
		void                                                                renderScreen( Texture2dPtr texture );

		void                                           bind( ShaderPtr shader, GeometryPtr geo = GeometryPtr() );
		void                                         unbind( ShaderPtr shader, GeometryPtr geo = GeometryPtr() );

		//
		// some standard resources
		//
		GeometryPtr                                         m_screenQuad;
		ShaderPtr                            m_simpleTextureShaderScreen;
		ShaderPtr                                  m_simpleTextureShader;
		ShaderPtr                                       m_constantShader;

	private:
		TransformState                                                                   m_currentTransformState;

		/*
		math::Matrix44f                                                                            m_modelMatrix; // object to world
		math::Matrix44f                                                                             m_viewMatrix; // world to camera
		math::Matrix44f                                                                        m_modelViewMatrix; // world to eye
		math::Matrix44f                                                                       m_projectionMatrix; // camera to view
		math::Matrix44f                                                              m_modelViewProjectionMatrix; // model view projection matrix (world to screen)
		math::Matrix44f                                                                      m_viewInverseMatrix; // view matrix inverse (camera to world)
		math::Matrix44f                                                                       m_modelViewInverse; // model view matrix inverse
		math::Matrix33f                                                              m_modelViewInverseTranspose; // model view matrix inverse transpose (model view matrix without scaling/shearing) used to transform vectors
		*/




		AttributePtr                                                                                   m_mvmAttr; // model view matrix (world to eye)
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
