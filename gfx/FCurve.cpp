#include "FCurve.h"
#include <algorithm>

namespace base
{

	//
	//
	//
	FCurve::FCurve( Type type ) : m_type(type), m_defaultValue(0.0f)
	{
	}

	FCurve::FCurve( const FCurve &curve )
	{
		m_type = curve.m_type;
		m_defaultValue = curve.m_defaultValue;
		m_values = std::vector<float>(curve.m_values);
		m_x = std::vector<float>(curve.m_x);
	}

	bool FCurve::isEmpty()
	{
		return m_x.empty();
	}

	void FCurve::clear()
	{
		m_x.clear();
		m_values.clear();
	}

	void FCurve::addCP( float x, float value )
	{
		m_x.push_back( x );
		m_values.push_back( value );
	}

	void FCurve::setCP( int index, float x, float value )
	{
		m_x[index] = x;
		m_values[index] = value;
	}

	//
	//
	//
	float FCurve::eval( float x ) const
	{
		float value = m_defaultValue;
		if( !m_values.empty() )
		{
			switch(m_type)
			{
			case LINEAR:
				math::evalLinear( &m_values[0], &m_x[0], (int)m_values.size(), 1, x, &value );
				break;
			case CATMULLROM:
			default:
				math::evalCatmullRom( &m_values[0], &m_x[0], (int)m_values.size(), 1, x, &value );
				break;
			}
		}
		return value;
	}


	template <typename T>
	std::vector<T> sort_from_ref(
		std::vector<T> const& in,
		std::vector<std::pair<std::size_t, typename std::vector<T>::const_iterator> > const& reference
	) {
		std::vector<T> ret(in.size());

		size_t const size = in.size();
		for (size_t i = 0; i < size; ++i)
			ret[i] = in[reference[i].first];

		return ret;
	}
/* // commented out because of compiler error with gcc
	//
	// sorts CP's after increasing x
	//
	void FCurve::sort()
	{
		typedef std::vector<float>::const_iterator xIter;

		// build list which associates x with index
		std::vector<std::pair<std::size_t, xIter> > order(m_x.size());

		std::size_t n = 0;
		for (xIter it = m_x.begin(); it != m_x.end(); ++it, ++n)
			order[n] = std::make_pair(n, it);



		// finally sort pair list(order) using custom comparator
		struct ordering
		{
			bool operator ()(std::pair<std::size_t, xIter> const& a, std::pair<std::size_t, xIter> const& b)
			{
				return a.second < b.second;
			}
		};
		std::sort(order.begin(), order.end(), ordering());

		// no sort all other vectors from ref
		sort_from_ref<float>( m_values, order );
	}
*/

}
