
#include "Op.h"



namespace base
{
	namespace ops
	{

		Op::Op() : m_invalid(true)
		{
		}

		// computes m_outputs
		void Op::execute()
		{
		}

		//
		// runs execute if necessary
		//
		void Op::validate()
		{
			if( m_invalid )
			{
				execute();
				m_invalid = false;
			}
		}

		MeshPtr Op::getMesh( int outputIndex )
		{
			validate();
			if( outputIndex >= m_outputs.size() )
				return MeshPtr();
			return std::dynamic_pointer_cast<Mesh>(m_outputs[outputIndex]);
		}
	}
}