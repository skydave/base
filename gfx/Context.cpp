#include "Context.h"

//#include "Attribute.h"
//#include "Shader.h"


namespace base
{

	Context::Context()
	{

		//
		// setup transform stuff
		//
		m_modelMatrix = math::Matrix44f::Identity();
		m_viewMatrix = math::Matrix44f::Identity();
		m_projectionMatrix = math::Matrix44f::Identity();
		m_viewInverseMatrix = math::Matrix44f::Identity();



		// view projection matrix
		m_mvpmAttr = AttributePtr(new Mat44Attribute() );
		m_mvpmAttr->appendElement( math::Matrix44f::Identity() );

		// model view matrix inverse transpose
		m_mvminvtAttr = AttributePtr(new Mat33Attribute() );
		m_mvminvtAttr->appendElement( math::Matrix33f::Identity() );

		// view matrix inverse (camera transform)
		m_vminvAttr = AttributePtr(new Mat44Attribute() );
		m_vminvAttr->appendElement( math::Matrix44f::Identity() );



		//
		// register predefined globals
		//
		m_globalUniforms.insert( std::make_pair( "mvpm", m_mvpmAttr ) );
		m_globalUniforms.insert( std::make_pair( "mvminvt", m_mvminvtAttr ) );
		m_globalUniforms.insert( std::make_pair( "vminv", m_vminvAttr ) );
		////Shader::setGlobalUniform( "vm", cam->viewMatrixAttribute ); // view matrix
		//Shader::setGlobalUniform( "vminv", cam->transformMatrixAttribute ); // inverse view matrix (camera world transform)
		//Shader::setGlobalUniform( "mm", mmAttr ); // model matrix (object to world transform)


		//
		m_screenQuad = geo_quad();

	}


	void Context::setView( const math::Matrix44f &view, const math::Matrix44f &viewInv, const math::Matrix44f &proj )
	{
		m_viewMatrix = view;
		m_viewInverseMatrix = viewInv;
		m_projectionMatrix = proj;

		// update camera transform
		m_vminvAttr->set( 0, m_viewInverseMatrix );

		//updateModelViewProjection();
		math::Matrix44f m = m_modelMatrix * m_viewMatrix * m_projectionMatrix;
		m_mvpmAttr->set( 0, m );

		//updateModelViewMatrixInverseTranspose()
		math::Matrix33f m33f; // result which will be uploaded

		m = m_modelMatrix * m_viewMatrix;
		m.invert();
		m.transpose();

		// we do the transpose when we extract the orientation from 44f matrix
		m33f.ma[0] = m.m[0][0];
		m33f.ma[1] = m.m[0][1];
		m33f.ma[2] = m.m[0][2];
		m33f.ma[3] = m.m[1][0];
		m33f.ma[4] = m.m[1][1];
		m33f.ma[5] = m.m[1][2];
		m33f.ma[6] = m.m[2][0];
		m33f.ma[7] = m.m[2][1];
		m33f.ma[8] = m.m[2][2];

		m_mvminvtAttr->set( 0, m33f );
	}








	void Context::render( GeometryPtr geo, ShaderPtr shader )
	{
		if(shader && shader->isOk())
		{

			shader->use();

			// iterate all active attributes and bind attributes from geometry
			for( std::map<std::string, int>::iterator it = shader->m_activeAttributes.begin(); it != shader->m_activeAttributes.end(); ++it )
			{
				const std::string &name = it->first;
				int attrIndex = it->second;
				// getAttr and bind(with index)
				if (geo->hasAttr(name))
					geo->getAttr(name)->bindAsAttribute(attrIndex);
			}

			// iterate all active uniforms
			for( std::map<std::string, int>::iterator it = shader->m_activeUniforms.begin(); it != shader->m_activeUniforms.end(); ++it )
			{
				const std::string &name = it->first;
				int uniformIndex = it->second;
				// if geometry has uniform attached
				if(0)
				{
					// use it
					// iterate all uniforms of the geometry
					//for( int j=0; j<m_uniforms.size(); ++j )
					//{
					//	char *t1 = (char *)shader->m_activeUniformNames.m_data[i];
					//	char *t2 = (char *)m_uniformNames.m_data[j];
					//	if( !strcmp( (char *)shader->m_activeUniformNames.m_data[i], m_uniformNames.m_data[j] ) )
					//		getUniform(j)->bindAsUniform( shader->m_activeUniforms.m_data[i] );
					//}
				}else
				// otherwise look in the globals to see if we can take it from there
				{
					std::map<std::string, AttributePtr>::iterator it = m_globalUniforms.find( name );

					if( it != m_globalUniforms.end() )
						it->second->bindAsUniform(uniformIndex);
				}
			}
		}


		// render primitives

		// ...using indexbufferobject (does not work on laptop)
		//oglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
		//oglBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer.size(), m_indexBuffer.m_data, GL_STATIC_DRAW);
		//glDrawElements(m_primitiveType, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

		// ...works for now
		glDrawElements(geo->m_primitiveType, (GLsizei)geo->m_indexBuffer.size(), GL_UNSIGNED_INT, &geo->m_indexBuffer[0]);


		if(shader && shader->isOk())
		{
			// iterate all active attributes
			// iterate all active attributes and unbind attributes
			for( std::map<std::string, int>::iterator it = shader->m_activeAttributes.begin(); it != shader->m_activeAttributes.end(); ++it )
			{
				const std::string &name = it->first;
				int attrIndex = it->second;
				// getAttr and bind(with index)
				if (geo->hasAttr(name))
					geo->getAttr(name)->unbindAsAttribute(attrIndex);
			}

			// disable shader
			glUseProgram(0);
		}
	}

	void Context::renderScreen( ShaderPtr shader )
	{
		render( m_screenQuad, shader );
	}







	/*



	void TransformManager::updateModelViewProjection()
	{
		math::Matrix44f m = m_modelMatrix * m_viewMatrix * m_projectionMatrix;
		m_mvpmAttr->setElement( 0, m.ma );
	}

	void TransformManager::updateModelViewMatrixInverseTranspose()
	{
		float m33f[9]; // result which will be uploaded

		math::Matrix44f m = m_modelMatrix * m_viewMatrix;
		m.invert();
		m.transpose();

		// we do the transpose when we extract the orientation from 44f matrix
		m33f[0] = m.m[0][0];
		m33f[1] = m.m[0][1];
		m33f[2] = m.m[0][2];
		m33f[3] = m.m[1][0];
		m33f[4] = m.m[1][1];
		m33f[5] = m.m[1][2];
		m33f[6] = m.m[2][0];
		m33f[7] = m.m[2][1];
		m33f[8] = m.m[2][2];

		m_mvminvtAttr->setElement( 0, m33f );
	}
	*/

}
