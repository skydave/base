
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

		// connects this operator as an input to the other operator
		void Op::plug( OpPtr other )
		{
			// append to generic inputs
			m_opList.push_back( other );
		}

		//
		// returns true if an operator has been associated with the specified input
		//
		bool Op::hasInput( const std::string &inputName )
		{
			if( m_opMap.find( inputName ) != m_opMap.end() )
				return true;
			return false;
		}

		//
		// returns the op which has been plugged to the specified name or invalid OpPtr if name does not exist
		//
		OpPtr Op::getInput( const std::string &inputName )
		{
			OpMap::iterator it = m_opMap.find( inputName );
			if( it != m_opMap.end() )
				return it->second;
			return OpPtr();
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
			return base::dynamic_pointer_cast<Mesh>(m_outputs[outputIndex]);
		}











		// ================================ GETTER ============================================

		void Op::get( const std::string &inputName, math::Matrix44f &value )
		{
			OpPtr op = getInput( inputName );
			// does input exist?
			if( op )
			{
				op->validate();
				//TODO: get output and check if its a variant object containing a matrix...
				asdasd
			}
		}
	}
}
