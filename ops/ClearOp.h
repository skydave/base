#include "Op.h"

extern float g_testValue;
namespace base
{
	namespace ops
	{
		// this operator allows the user to have any generic function to be executed from an op
		BASE_DECL_SMARTPTR(ClearOp);
		class ClearOp : public Op
		{
		public:

			ClearOp() : Op()
			{
			}
			virtual ~ClearOp()
			{
			}

			// executes the function
			virtual void execute()
			{
				math::Vec3f color( 0.3f, 0.3f, 0.3f );
				get( "color", color );

				//glClearColor( color.x, color.y, color.z, 1.0f );
				glClearColor( g_testValue, g_testValue, g_testValue, 1.0f );
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			static ClearOpPtr create()
			{
				ClearOpPtr clearOp( new ClearOp() );
				return clearOp;
			}
		};
	}
}

