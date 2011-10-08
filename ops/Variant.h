#pragma once


#include <util/Object.h>

#include <ttl/var/variant.hpp>

#include <math/Matrix44f.h>


namespace base
{
	// this class wraps a variant into an object to be used by ops
	BASE_DECL_SMARTPTR(Variant);
	class Variant : public Object
	{
	public:
		typedef ttl::var::variant<float, double, int, math::Matrix44f, math::Vec3f> variant_t;

		Variant() : Object()
		{
		}

		template<typename T>
		void get( T &value )
		{
			value = ttl::var::get<T>( m_variant );
		}

		static VariantPtr create()
		{
			return VariantPtr( new Variant() );
		}

		variant_t m_variant;
	};
}









