
#include "Op.h"
#include "ttl/func/function.hpp"



namespace base
{
	namespace ops
	{
		// this operator allows the user to have any generic function to be executed from an op
		BASE_DECL_SMARTPTR(FuncOp);
		class FuncOp : public Op
		{
		public:
			typedef ttl::func::function<void> function_t;

			FuncOp() : Op()
			{
			}
			virtual ~FuncOp()
			{
			}

			// executes the function
			virtual void execute()
			{
				if( m_function.is_valid() )
					m_function();
			}

			static FuncOpPtr create( void (*func)() )
			{
				FuncOpPtr funcOp( new FuncOp() );
				funcOp->m_function = func;
				return funcOp;
			}

			function_t m_function;
		};
	}
}






