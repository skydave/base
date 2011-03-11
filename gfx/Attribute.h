#pragma once
#include <vector>

#include <util/shared_ptr.h>

#define Vec3Attribute() Attribute(3, sizeof(float))
#define Vec2Attribute() Attribute(2, sizeof(float))

namespace base
{

	BASE_DECL_SMARTPTR(Attribute);

	// basicly a list manager
	struct Attribute
	{
		Attribute( char numComponents=3, char componentSize=sizeof(float) ) : m_numElements(0), m_numComponents(numComponents), m_componentSize(componentSize)
		{
			//oglGenBuffers(1, &m_bufferId);

			//m_componentType = GL_FLOAT;
		}

		//Attribute *copy();

		template<typename T>
		int appendElement( const T &value );

		template<typename T>
		int appendElement( const T &v0, const T &v1 );

		template<typename T>
		int appendElement( const T &v0, const T &v1, const T &v2 );

		template<typename T>
		T &get( int index );

		//int appendElement( void *mem );
		//int appendElements( int num );
		//void getElement( int vertexIndex, void *mem );
		//void setElement( int vertexIndex, void *mem );

		//void removeElement( int vertexIndex );

		void clear()
		{
			m_data.clear();
			m_numElements = 0;
		}


		int numElements()
		{
			return m_numElements;
		}

		int numComponents()
		{
			return m_numComponents;
		}

		int elementComponentType()
		{
			return m_componentType;
		}

		int elementComponentSize()
		{
			return m_componentSize;
		}

		void *getRawPointer()
		{
			return (void *)&m_data[0];
		}
		void *getRawPointer( int index )
		{
			// should change on e per type basis
			return (void *)&m_data[index*numComponents()*elementComponentSize()];
		}


		//
		// OpenGL
		//
		//virtual void bindAsAttribute( int index );
		//virtual void unbindAsAttribute( int index );
		//virtual void bindAsUniform( int index );
		//virtual void unbindAsUniform( int index );


		std::vector<unsigned char> m_data;
		char m_componentSize; // size in memory of a component of an element in byte
		int m_componentType;
		char m_numComponents; // number of components per element
		int m_numElements;


		// OpenGL
		//unsigned int m_bufferId;
	};


	template<typename T>
	int Attribute::appendElement( const T &value )
	{
		int pos = m_data.size();
		m_data.resize( pos + sizeof(T) );
		*((T *)&m_data[pos]) = value;
		++m_numElements;
		return m_numElements;
	}

	template<typename T>
	int Attribute::appendElement( const T &v0, const T &v1 )
	{
		int pos = m_data.size();
		m_data.resize( pos + sizeof(T)*2 );
		T *data = (T*)&m_data[pos];
		*data = v0;++data;
		*data = v1;++data;
		++m_numElements;
		return m_numElements;
	}

	template<typename T>
	int Attribute::appendElement( const T &v0, const T &v1, const T &v2 )
	{
		int pos = m_data.size();
		m_data.resize( pos + sizeof(T)*3 );
		T *data = (T*)&m_data[pos];
		*data = v0;++data;
		*data = v1;++data;
		*data = v2;++data;
		++m_numElements;
		return m_numElements;
	}

	template<typename T>
	T &Attribute::get( int index )
	{
		T *data = (T*)&m_data[index * sizeof(T)];
		return *data;
	}


} // namespace base
