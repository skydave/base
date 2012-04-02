
#include "Op.h"
#include "ttl/func/function.hpp"



namespace base
{
	namespace ops
	{
		// ============================================================
		// FuncOp
		// this operator allows the user to have any generic function to be executed from an op
		// ============================================================

		// this operator allows the user to have any generic function to be executed from an op
		BASE_DECL_SMARTPTR(FuncOp);
		class FuncOp : public Op
		{
		public:
			typedef ttl::func::function<void> function_t;

			FuncOp() : Op(){}
			virtual ~FuncOp(){}

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


		// ============================================================
		// NOP
		// does nothing
		// ============================================================

		BASE_DECL_SMARTPTR(NOP);
		class NOP : public Op
		{
		public:
			NOP() : Op(){}
			virtual ~NOP(){}

			// executes the function
			virtual void execute()
			{
			}

			static NOPPtr create()
			{
				return NOPPtr( new NOP() );
			}

		};

		// ============================================================
		// SequenceOp
		// executes all input ops
		// ============================================================

		BASE_DECL_SMARTPTR(SequenceOp);
		class SequenceOp : public Op
		{
		public:
			SequenceOp() : Op(){}
			virtual ~SequenceOp(){}

			// executes the function
			virtual void execute()
			{
				// execute inputs
				for( OpList::iterator it = m_opList.begin(); it != m_opList.end(); ++it)
					(*it)->execute();
			}

			static SequenceOpPtr create()
			{
				return SequenceOpPtr( new SequenceOp() );
			}

		};
	}
}

