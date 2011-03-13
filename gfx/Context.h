#pragma once
#include <math/Math.h>
#include <util/shared_ptr.h>

#include "Attribute.h"
#include "Shader.h"
#include "Geometry.h"



namespace base
{
	BASE_DECL_SMARTPTR(Context);
	struct Context
	{

		Context();


		//
		// transform matrix management
		//
		void setView( const math::Matrix44f &view, const math::Matrix44f &viewinv, const math::Matrix44f &proj );

		math::Matrix44f           m_modelMatrix; // object to world
		math::Matrix44f     m_viewInverseMatrix; // world to camera
		math::Matrix44f            m_viewMatrix; // camera to world
		math::Matrix44f      m_projectionMatrix; // camera to view


		AttributePtr    m_mvpmAttr; // model view projection matrix
		AttributePtr   m_vminvAttr; // view matrix inverse
		AttributePtr m_mvminvtAttr; // model view matrix inverse transpose (model view matrix without scaling/shearing) used to transform vectors

		//
		// global uniform manangement
		//
		std::map<std::string, AttributePtr> m_globalUniforms;


		//
		// rendering
		//
		void render( GeometryPtr geo, ShaderPtr shader );
		void renderScreen( ShaderPtr shader );

		GeometryPtr                         m_screenQuad;

		/*
		void popCamera();


		//void pushModelMatrix( math::Matrix44f modelMatrix = math::Matrix44f::Identity() );
		//void popModelMatrix();

		void updateModelViewProjection();
		void updateModelViewMatrixInverseTranspose();



		math::Matrix44f m_modelMatrix;
		math::Matrix44f m_viewMatrix;
		math::Matrix44f m_viewInverseMatrix;
		math::Matrix44f m_projectionMatrix;





		//Attribute *projectionMatrixAttribute;
		//Attribute *viewMatrixAttribute;
		//Attribute *viewProjectionMatrixAttribute;
		//Attribute *transformMatrixAttribute;
		*/
	};
}