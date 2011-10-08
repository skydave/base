#pragma once


//
#define BASE_DECL_SMARTPTR_STRUCT(C)\
	struct C;\
	BASE_DECL_SMARTPTR_(C)

//
#define BASE_DECL_SMARTPTR(C)\
	class C;\
	BASE_DECL_SMARTPTR_(C)

//
#define BASE_DECL_SMARTPTR_(C)\
	typedef base::shared_ptr<C> C##Ptr;\
	typedef base::shared_ptr<const C> C##CPtr;

#ifdef _WINDOWS
#include <memory>
#endif
#ifdef linux
#include <tr1/memory>
#endif

namespace base
{
	//smart pointers
	using std::tr1::shared_ptr;

	using std::tr1::dynamic_pointer_cast;

	using std::tr1::enable_shared_from_this;

/*
	//shared-pointer-from-this
	using boost::enable_shared_from_this;

	//shared pointer casts
	using boost::dynamic_pointer_cast;
	using boost::static_pointer_cast;
	using boost::const_pointer_cast;

	//lift get_deleter and get_pointer
	using boost::get_pointer;
	using boost::get_deleter;
*/
}
