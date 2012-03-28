#pragma once


#if (defined _MSC_VER)
typedef signed __int64        sint64;
typedef unsigned __int64      uint64;
typedef unsigned __int32      uint32;
#else
typedef unsigned long long    uint64;
typedef signed long long      sint64;
typedef unsigned int          uint32;
#endif
