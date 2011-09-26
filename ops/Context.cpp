#include "Context.h"


#include <gfx/Context.h>


extern base::ContextPtr context;


namespace base
{
	namespace ops
	{

		std::stack<float> Context::m_time;

		// pushes a new time onto the stack
		void Context::pushTime( float time )
		{
			m_time.push( time );
		}

		// removes time from the time stack
		float Context::popTime()
		{
			float t = time();
			m_time.pop();
			return t;
		}

		// returns current time, if stack is empty it will return base::Context::getUniform("time")
		float Context::time()
		{
			if( m_time.empty() )
				return context->getUniform("time")->get<float>(0);
			return m_time.top();
		}

	}
}