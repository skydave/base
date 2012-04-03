#include "Op.h"


namespace base
{
	namespace ops
	{
		// ============================================================
		// ClearOp
		// wraps glClear
		// ============================================================

		BASE_DECL_SMARTPTR(ClearOp);
		class ClearOp : public Op
		{
		public:
			ClearOp() : Op(){}
			virtual ~ClearOp(){}

			// executes the function
			virtual void execute()
			{
				math::Vec3f color( 0.3f, 0.3f, 0.3f );
				get( "color", color );

				glClearColor( color.x, color.y, color.z, 1.0f );
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			static ClearOpPtr create()
			{
				return ClearOpPtr( new ClearOp() );
			}
		};
	}
}
