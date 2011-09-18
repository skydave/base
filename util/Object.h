#pragma once

#include "shared_ptr.h"





namespace base
{
	BASE_DECL_SMARTPTR(Object);
	class Object
	{
	public:
		virtual ~Object();
	};
}