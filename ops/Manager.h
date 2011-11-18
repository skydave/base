#pragma once


#include <stack>
#include <gfx/Context.h>




namespace base
{
	namespace ops
	{
		//
		class Manager
		{
		public:
			static base::ContextPtr                      context();
			static     void setContext( base::ContextPtr context ); // set graphics context





		private:
			static base::ContextPtr                      m_context;
		};
	}
}
