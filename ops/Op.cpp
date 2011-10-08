
#include "Op.h"
#include "Variant.h"


namespace base
{
	namespace ops
	{

		Op::Op() : Object(), m_invalid(true)
		{
		}

		// computes m_outputs
		void Op::execute()
		{
		}

		// connects this operator as an input to the other operator
		void Op::plug( OpPtr other )
		{
			other->addInput( dynamic_pointer_cast<Op>(shared_from_this()) );
		}

		// connects this operator as an input to the other operator
		void Op::plug( OpPtr other, const std::string &inputName )
		{
			other->setInput( dynamic_pointer_cast<Op>(shared_from_this()), inputName );
		}

		void Op::addInput( OpPtr other )
		{
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
		// returns true if an operator has been associated with the specified input
		//
		void Op::setInput( OpPtr other, const std::string &inputName )
		{
			m_opMap[inputName] = other;
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

		ObjectPtr Op::getOutput( int index )
		{
			return m_outputs[index];
		}

		//
		// runs execute if necessary
		//
		void Op::validate()
		{
			if( m_invalid )
			{
				execute();
				//m_invalid = false;
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

				VariantPtr var = dynamic_pointer_cast<Variant>(op->getOutput( 0 ));

				if( var )
					var->get( value );
			}
		}

		void Op::get( const std::string &inputName, math::Vec3f &value )
		{
			OpPtr op = getInput( inputName );
			// does input exist?
			if( op )
			{
				op->validate();

				VariantPtr var = dynamic_pointer_cast<Variant>(op->getOutput( 0 ));

				if( var )
					var->get( value );
			}
		}

		void Op::get( const std::string &inputName, float &value )
		{
			OpPtr op = getInput( inputName );
			// does input exist?
			if( op )
			{
				op->validate();

				VariantPtr var = dynamic_pointer_cast<Variant>(op->getOutput( 0 ));

				if( var )
					var->get( value );
			}
		}
	}
}
