#include "Attribute.h"

#include <gltools/gl.h>

namespace base
{
	Attribute::Attribute( char numComponents, ComponentType componentType ) : m_numElements(0), m_numComponents(numComponents), m_componentType(componentType)
	{
		switch(componentType)
		{
		case SAMPLER:
		case INT:m_componentSize=sizeof(int);break;
		default:
		case FLOAT:m_componentSize=sizeof(float);break;
		};

		glGenBuffers(1, &m_bufferId);
	}


	void Attribute::bindAsAttribute( int index )
	{
		// activate and specify pointer to vertex array
		// should be done only when attribute has been updated
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
		glBufferData(GL_ARRAY_BUFFER, numComponents()*elementComponentSize()*numElements(), getRawPointer(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, numComponents(), elementComponentType(), false, 0, 0);

	}
	void Attribute::unbindAsAttribute( int index )
	{
		// deactivate vertex arrays after drawing
		glDisableVertexAttribArray(index);
	}

	void Attribute::bindAsUniform( int index )
	{
		switch( numComponents() )
		{
		case 1:
			if( elementComponentType() == FLOAT )
			{
				//printf("setting uniform tmp: %f at uniform location %i\n", *((float *)getRawPointer()), index );
				glUniform1fv( index, numElements(), (float *)getRawPointer());
			}
			else if( elementComponentType() == INT )
			{
				glUniform1iv( index, numElements(), (int*)getRawPointer());
			}else if( elementComponentType() == SAMPLER )
			{
				// get gl textureid
				unsigned int t = (unsigned int) (*(int*)getRawPointer());

				// bind texture to given texture unit (index)
				glActiveTexture(GL_TEXTURE0+index);
				glBindTexture(GL_TEXTURE_2D, t);

				// now set the sampler uniform to point to the textureunit
				int tt = index; // for now texture unit == unfiform location
								// this will be bad with higher number of uniforms in shader
								// need more clever texture unit management
				glUniform1iv( index, 1, &tt);
			}
			break;
		case 2:
			if( elementComponentType() == GL_FLOAT )
				glUniform2fv( index, numElements(), (float *)getRawPointer());
			break;
		case 3:
			if( elementComponentType() == GL_FLOAT )
				glUniform3fv( index, numElements(), (float *)getRawPointer());
			break;
		case 4:
			if( elementComponentType() == GL_FLOAT )
				glUniform4fv( index, numElements(), (float *)getRawPointer());
			break;
		case 9:
			glUniformMatrix3fv( index, numElements(), false, (float *)getRawPointer() );
			break;
		case 16:
			glUniformMatrix4fv( index, numElements(), false, (float *)getRawPointer() );
			break;
		};
	}
	void Attribute::unbindAsUniform( int index )
	{
		// ?
		// for better texture unit management tell the currently used texture unit is not used anymore
	}

/*
Attribute *Attribute::copy()
{
	Attribute *nattr = new Attribute( numComponents(), elementComponentSize() );
	nattr->m_data.resize( numElements() * numComponents()*elementComponentSize() );
	msys_memcpy( nattr->m_data.m_data, m_data.m_data, numElements() * numComponents()*elementComponentSize() );
	nattr->m_numElements = numElements();
	return nattr;
}




math::Vec3f Attribute::getVec3f( int vertexIndex )
{
	float *data = (float*)&m_data.m_data[vertexIndex * sizeof(math::Vec3f)];
	float f0 = *data;data++;
	float f1 = *data;data++;
	float f2 = *data;
	return math::Vec3f( f0, f1, f2 );
}

void Attribute::getElement( int vertexIndex, void *mem )
{
	if(vertexIndex<numElements())
		msys_memcpy( mem, &m_data.m_data[vertexIndex * numComponents()*elementComponentSize()], numComponents()*elementComponentSize() );
}

int Attribute::appendElement( void *mem )
{
	int pos = m_data.size();
	m_data.resize( pos + numComponents()*elementComponentSize() );
	msys_memcpy( &m_data.m_data[pos], mem, numComponents()*elementComponentSize() );
	return m_numElements++;
}

int Attribute::appendElements( int num )
{
	m_data.resize( m_data.size() + num*numComponents()*elementComponentSize() );
	m_numElements += num;
	return m_numElements;
}

void Attribute::setElement( int vertexIndex, void *mem )
{
	if(vertexIndex<numElements())
		msys_memcpy( &m_data.m_data[vertexIndex * numComponents()*elementComponentSize()], mem, numComponents()*elementComponentSize() );
}


void Attribute::removeElement( int vertexIndex )
{
	m_data.erase( vertexIndex*numComponents()*elementComponentSize(), numComponents()*elementComponentSize() );
	--m_numElements;
}

*/

}
