#pragma once
#include <vector>
#include <math/Math.h>

#include <util/shared_ptr.h>






namespace base
{
	BASE_DECL_SMARTPTR(FCurve);

	struct FCurve
	{
		FCurve();

		void addKey( float x, float value );

		float eval( float x );

		std::vector<float> m_values;
		std::vector<float> m_x;


		float defaultValue;
	};
}

