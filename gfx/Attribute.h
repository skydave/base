#pragma once
#include <vector>
#include <math/Math.h>
#include <util/shared_ptr.h>



namespace base
{

	BASE_DECL_SMARTPTR_STRUCT(Attribute);

	// basicly a list manager
	struct Attribute
	{
		enum ComponentType
		{
			INT,
			FLOAT,
			SAMPLER
			/*
			SAMPLER1D,
			SAMPLER2D,
			SAMPLER2DMultisample,
			SAMPLER2DARRAY,
			SAMPLER3D,
			SAMPLERCUBE
			*/
		};

		Attribute( char numComponents=3, ComponentType componentType = FLOAT, int textureTarget = 0 );
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
			m_isDirty = true;
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

		int elementComponentType();

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
		int               m_componentType;
		int               m_textureTarget; // will hold the textureTarget when componentType is sampler
		char              m_numComponents; // number of components per element
		int                 m_numElements;

		//
		// static creators
		//
		static AttributePtr                    createSamplerCube();
		static AttributePtr                      createSampler3d();
		static AttributePtr                      createSampler2d();
		static AttributePtr                    createSampler2dMS();
		static AttributePtr                 createSampler2dArray();
		static AttributePtr                      createSampler1d();
		static AttributePtr                          createMat33();
		static AttributePtr                          createMat44();
		static AttributePtr     createVec4f( int numElements = 0 );
		static AttributePtr     createVec3f( int numElements = 0 );
		static AttributePtr     createVec2f( int numElements = 0 );
		static AttributePtr                          createFloat();
		static AttributePtr                            createInt();

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
		m_data.resize( pos + sizeof(T)*4 );
		T *data = (T*)&m_data[pos];
		*data = v0;++data;
		*data = v1;++data;
		*data = v2;++data;
		*data = v3;++data;
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
		m_isDirty = true;
	}

	template<typename T>
	void Attribute::set( unsigned int index, T v0, T v1, T v2 )
	{
		T *data = (T*)&m_data[index * sizeof(T) * 3];
		*data++ = v0;
		*data++ = v1;
		*data++ = v2;
		m_isDirty = true;
	}

	template<typename T>
	void Attribute::set( unsigned int index, T v0, T v1, T v2, T v3 )
	{
		T *data = (T*)&m_data[index * sizeof(T) * 4];
		*data++ = v0;
		*data++ = v1;
		*data++ = v2;
		*data++ = v3;
		m_isDirty = true;
	}

} // namespace base
