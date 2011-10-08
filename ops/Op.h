#pragma once

#include <gfx/Mesh.h>
#include <util/shared_ptr.h>
#include <list>






namespace base
{
	namespace ops
	{
		BASE_DECL_SMARTPTR(Op);
		class Op : public Object
		{
		public:
			Op();                                               // constructor

			virtual void                                                                                 execute(); // computes m_outputs

			void                                                                               plug( OpPtr other ); // connects this operator as an input to the other operator
			void                                                 plug( OpPtr other, const std::string &inputName ); // connects this operator as an input to the other operator

			void                                                                           addInput( OpPtr other ); //
			void                                             setInput( OpPtr other, const std::string &inputName ); //
			bool                                                          hasInput( const std::string &inputName ); // returns true if an operator has been associated with the specified input
			OpPtr                                                         getInput( const std::string &inputName ); // returns the op which has been plugged to the specified name or invalid OpPtr if name does not exist

			ObjectPtr                                                                       getOutput( int index );



			MeshPtr                                                                     getMesh( int outputIndex );
			void                                       get( const std::string &inputName, math::Matrix44f &value );
			void                                           get( const std::string &inputName, math::Vec3f &value );
			void                                                 get( const std::string &inputName, float &value );
		protected:
			typedef std::list<OpPtr>                                                                        OpList;
			typedef std::map<std::string, OpPtr>                                                             OpMap;
			void                                                                                        validate(); // runs execute if necessary
			bool                                                                                         m_invalid; // dirty flag

			OpList                                                                                        m_opList; // operators which have been connected without associated name
			OpMap                                                                                          m_opMap; // operators which have been connected with! associated name
			std::vector<ObjectPtr>                                                                       m_outputs; // results
		};
	}
}
