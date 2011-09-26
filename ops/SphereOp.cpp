#include "SphereOp.h"








namespace base
{
	namespace ops
	{

		SphereOp::SphereOp( float radius ) : Op()
		{
			m_radius = radius;
		}

		SphereOp::~SphereOp()
		{
		}

		// creates a sphere mesh
		void SphereOp::execute()
		{
			if( m_outputs.size() != 1 )
				m_outputs.push_back( Mesh::sphere(m_radius) );
		}

		SphereOpPtr SphereOp::create( float radius )
		{
			return SphereOpPtr( new SphereOp(radius) );
		}

	}
}
