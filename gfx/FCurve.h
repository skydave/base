#pragma once
#include <vector>
#include <math/Math.h>

#include <util/shared_ptr.h>






namespace base
{
	BASE_DECL_SMARTPTR(FCurve);

	struct FCurve
	{
		enum Type
		{
			LINEAR,
			CATMULLROM
		};
		FCurve(Type type = CATMULLROM);
		FCurve( const FCurve &curve );

		bool isEmpty();
		void clear();

		void addCP( float x, float value );
		void setCP( int index, float x, float value );

		float eval( float x ) const;

		std::vector<float> m_values;
		std::vector<float> m_x;


		float m_defaultValue;
		Type          m_type;

	private:
		void sort(); // sorts CP's after increasing x
	};
}

