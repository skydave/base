#include "Op.h"

#include "Variant.h"

namespace base
{
	namespace ops
	{
		// this operator allows the user to have any generic function to be executed from an op
		BASE_DECL_SMARTPTR(OrbitTransformOp);
		class OrbitTransformOp : public Op
		{
		public:

			OrbitTransformOp() : Op()
			{
			}
			virtual ~OrbitTransformOp()
			{
			}

			// executes the function
			virtual void execute()
			{
				float azimuth = 0.0f;
				float elevation = 0.0f;
				float distance = 10.0f;
				math::Vec3f lookAt;

				get( "azimuth", azimuth);
				get( "elevation", elevation );
				get( "distance", distance );
				get( "lookAt", lookAt );

				math::Matrix44f m = math::Matrix44f::Identity();

				m.translate( math::Vec3f( 0.0f, 0.0f, distance ) );	

				//m.rotateZ( twist ); // not used
				m.rotateX( math::degToRad(elevation) );
				m.rotateY( math::degToRad(azimuth) );

				m.translate( lookAt );

				if(m_outputs.empty())
					m_outputs.push_back( Variant::create() );

				dynamic_pointer_cast<Variant>(m_outputs[0])->m_variant = m;
			}

			static OrbitTransformOpPtr create()
			{
				OrbitTransformOpPtr orbitTransformOp( new OrbitTransformOp() );
				return orbitTransformOp;
			}
		};
	}
}

