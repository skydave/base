#include "FCurve.h"


namespace base
{

	//
	//
	//
	FCurve::FCurve()
	{
		defaultValue = 0.0f;
	}

	void FCurve::addKey( float x, float value )
	{
		m_x.push_back( x );
		m_values.push_back( value );
	}

	//
	//
	//
	float FCurve::eval( float x )
	{
		float value = defaultValue;
		if( m_numKeys > 0 )
			math::evalCatmullRom( &m_values[0], &m_x[0], (int)m_values.size(), 1, x, &value );
		return value;
	}

}
