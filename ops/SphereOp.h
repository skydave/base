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
			SphereOp();
			virtual ~SphereOp();

			virtual void                             execute(); // creates a sphere mesh

			static SphereOpPtr create();
		private:


		};
	}
}