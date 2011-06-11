#pragma once
#include <vector>
#include <math/Math.h>
#include <util/shared_ptr.h>

/*
#define Vec3Attribute() Attribute(3, Attribute::FLOAT)
#define Vec2Attribute() Attribute(2, Attribute::FLOAT)
#define Mat44Attribute() Attribute(16, Attribute::FLOAT)
#define Mat33Attribute() Attribute(9, Attribute::FLOAT)
#define Sampler1dAttribute() Attribute(1, Attribute::SAMPLER1D)
#define Sampler2dAttribute() Attribute(1, Attribute::SAMPLER2D)
#define Sampler3dAttribute() Attribute(1, Attribute::SAMPLER3D)
*/

namespace base
{

	BASE_DECL_SMARTPTR(Attribute);

	// basicly a list manager
	struct Attribute
	{
		enum ComponentType
		{
			INT = 0x1404,
			FLOAT = 0x1406,
			SAMPLER1D,
			SAMPLER2D,
			SAMPLER3D
		};

		Attribute( char numComponents=3, ComponentType componentType = FLOAT );
		~Attribute();

		//Attribute *copy();

		template<typename T>
		unsigned int appendElement( const T &value );
		template<typename T>
		unsigned int appendElement( const T &v0, const T &v1 );
		template<typename T>
		unsigned int appendElement( const T &v0, const T &v1, const T &v2 );
		template<typename T>
		unsigned int appendElement( const T &v0, const T &v1, const T &v2, const T &v3 );

		template<typename T>
		T &get( unsigned int index );

		template<typename T>
		void set( unsigned int index, T value );
		template<typename T>
		void set( unsigned int index, T v0, T v1, T v2 );
		template<typename T>
		void set( unsigned int index, T v0, T v1, T v2, T v3 );

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

		void resize( int numElements )
		{
			m_data.resize( numElements*numComponents()*elementComponentSize() );
			m_numElements = numElements;
			m_isDirty = true;
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





		std::vector<unsigned char> m_data;
		char              m_componentSize; // size in memory of a component of an element in byte
		ComponentType     m_componentType;
		char              m_numComponents; // number of components per element
		int                 m_numElements;

		//
		// static creators
		//
		static AttributePtr createSampler3d();
		static AttributePtr createSampler2d();
		static AttributePtr createSampler1d();
		static AttributePtr     createMat33();
		static AttributePtr     createMat44();
		static AttributePtr     createVec4f();
		static AttributePtr     createVec3f( int numElements = 0 );
		static AttributePtr     createVec2f( int numElements = 0 );
		static AttributePtr     createFloat();
		static AttributePtr       createInt();

		//
		// OpenGL
		//
		void bindAsAttribute( int index );
		void unbindAsAttribute( int index );
		void bindAsUniform( int index );
		void unbindAsUniform( int index );

		unsigned int m_bufferId;
		bool          m_isDirty;
	};


	template<typename T>
	unsigned int Attribute::appendElement( const T &value )
	{
		unsigned int pos = (unsigned int) m_data.size();
		m_data.resize( pos + sizeof(T) );
		*((T *)&m_data[pos]) = value;
		return m_numElements++;
	}

	template<typename T>
	unsigned int Attribute::appendElement( const T &v0, const T &v1 )
	{
		unsigned int pos = (unsigned int) m_data.size();
		m_data.resize( pos + sizeof(T)*2 );
		T *data = (T*)&m_data[pos];
		*data = v0;++data;
		*data = v1;++data;
		return m_numElements++;
	}

	template<typename T>
	unsigned int Attribute::appendElement( const T &v0, const T &v1, const T &v2 )
	{
		unsigned int pos = (unsigned int) m_data.size();
		m_data.resize( pos + sizeof(T)*3 );
		T *data = (T*)&m_data[pos];
		*data = v0;++data;
		*data = v1;++data;
		*data = v2;++data;
		return m_numElements++;
	}

	template<typename T>
	unsigned int Attribute::appendElement( const T &v0, const T &v1, const T &v2, const T &v3 )
	{
		unsigned int pos = (unsigned int) m_data.size();
		m_data.resize( pos + sizeof(T)*3 );
		T *data = (T*)&m_data[pos];
		*data = v0;++data;
		*data = v1;++data;
		*data = v2;++data;
		return m_numElements++;
	}

	template<typename T>
	T &Attribute::get( unsigned int index )
	{
		T *data = (T*)&m_data[index * sizeof(T)];
		return *data;
	}

	template<typename T>
	void Attribute::set( unsigned int index, T value )
	{
		T *data = (T*)&m_data[index * sizeof(T)];
		*data = value;
	}

	template<typename T>
	void Attribute::set( unsigned int index, T v0, T v1, T v2 )
	{
		T *data = (T*)&m_data[index * sizeof(T) * 3];
		*data++ = v0;
		*data++ = v1;
		*data++ = v2;
	}

	template<typename T>
	void Attribute::set( unsigned int index, T v0, T v1, T v2, T v3 )
	{
		T *data = (T*)&m_data[index * sizeof(T) * 4];
		*data++ = v0;
		*data++ = v1;
		*data++ = v2;
		*data++ = v3;
	}

} // namespace base
