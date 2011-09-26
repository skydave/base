#pragma once

#include "Op.h"






namespace base
{
	namespace ops
	{
		BASE_DECL_SMARTPTR(SphereOp);
		class SphereOp : public Op
		{
		public:
			SphereOp( float radius = 1.0f );
			virtual ~SphereOp();

			virtual void                             execute(); // creates a sphere mesh

			static SphereOpPtr create( float radius = 1.0f );
		private:

			float m_radius;


		};
	}
}
