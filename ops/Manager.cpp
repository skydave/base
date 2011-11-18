#include "Manager.h"


#include <gfx/Context.h>



namespace base
{
	namespace ops
	{
		base::ContextPtr          Manager::m_context;

		void Manager::setContext( base::ContextPtr context )
		{
			m_context = context;
		}

		base::ContextPtr Manager::context()
		{
			return m_context;
		}


	}
}
