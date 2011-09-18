#pragma once

#include <gfx/Mesh.h>
#include <util/shared_ptr.h>
#include <vector>






namespace base
{
	namespace ops
	{
		class Op
		{
		public:
			Op();                                               // constructor

			virtual void                             execute(); // computes m_outputs



			MeshPtr                 getMesh( int outputIndex );
		protected:
			void                                    validate(); // runs execute if necessary
			bool                                     m_invalid; // dirty flag
			std::vector<ObjectPtr>                   m_outputs; // results
		};
	}
}