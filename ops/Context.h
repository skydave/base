#pragma once


#include <stack>




namespace base
{
	namespace ops
	{
		//
		// This class holds cross Operator information
		//
		class Context
		{
		public:
			static void                    pushTime( float time ); // pushes a new time onto the stack
			static float                                popTime(); // removes time from the time stack
			static float                                   time(); // returns current time, if stack is empty it will return base::Context::getUniform("time")




		private:
			static std::stack<float>                      m_time;
		};
	}
}
