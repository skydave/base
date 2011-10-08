#pragma once

#include "shared_ptr.h"





namespace base
{
	BASE_DECL_SMARTPTR(Object);
	class Object : public enable_shared_from_this<Object>
	{
	public:
		Object() : enable_shared_from_this<Object>()
		{
		}
		virtual ~Object()
		{
		}
	};
}
