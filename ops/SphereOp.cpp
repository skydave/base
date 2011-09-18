#include "SphereOp.h"








namespace base
{
	namespace ops
	{

		SphereOp::SphereOp() : Op()
		{
		}

		SphereOp::~SphereOp()
		{
		}

		// creates a sphere mesh
		void SphereOp::execute()
		{
			if( m_outputs.size() != 1 )
				m_outputs.push_back( Mesh::sphere() );
		}

		SphereOpPtr SphereOp::create()
		{
			return SphereOpPtr( new SphereOp() );
		}

	}
}
