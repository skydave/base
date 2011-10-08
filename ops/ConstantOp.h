#include "Op.h"

#include "Variant.h"

namespace base
{
	namespace ops
	{
		// this operator allows the user to have any generic function to be executed from an op
		BASE_DECL_SMARTPTR(ConstantOp);
		class ConstantOp : public Op
		{
		public:

			ConstantOp() : Op()
			{
			}
			virtual ~ConstantOp()
			{
			}

			// executes the function
			virtual void execute()
			{
				if(m_outputs.empty())
					m_outputs.push_back( Variant::create() );

				dynamic_pointer_cast<Variant>(m_outputs[0])->m_variant = m_variant;
			}

			static ConstantOpPtr create()
			{
				ConstantOpPtr constOp( new ConstantOp() );
				return constOp;
			}

			Variant::variant_t                 m_variant;
		};
	}
}

