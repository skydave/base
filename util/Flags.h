#pragma once

#include <util/types.h>




namespace base
{
	
	template<typename Enum>
	class Flags
	{
		typedef void **Zero;
		int i;
	public:
		typedef Enum enum_type;
		inline Flags(const Flags &f) : i(f.i) {}
		inline Flags(Enum f) : i(f) {}
		inline Flags(Zero = 0) : i(0) {}
		//inline Flags(Flags f) : i(f) {}

		inline Flags &operator=(const Flags &f) { i = f.i; return *this; }
		inline Flags &operator&=(int mask) { i &= mask; return *this; }
		//inline Flags &operator&=(uint32 mask) { i &= mask; return *this; }
		inline Flags &operator|=(Flags f) { i |= f.i; return *this; }
		inline Flags &operator|=(Enum f) { i |= f; return *this; }
		inline Flags &operator^=(Flags f) { i ^= f.i; return *this; }
		inline Flags &operator^=(Enum f) { i ^= f; return *this; }

		inline operator int() const { return i; }

		inline Flags operator|(Flags f) const { return Flags(Enum(i | f.i)); }
		inline Flags operator|(Enum f) const { return Flags(Enum(i | f)); }
		inline Flags operator^(Flags f) const { return Flags(Enum(i ^ f.i)); }
		inline Flags operator^(Enum f) const { return Flags(Enum(i ^ f)); }
		inline Flags operator&(int mask) const { return Flags(Enum(i & mask)); }
		//inline Flags operator&(uint mask) const { return Flags(Enum(i & mask)); }
		inline Flags operator&(Enum f) const { return Flags(Enum(i & f)); }
		inline Flags operator~() const { return Flags(Enum(~i)); }

		inline bool operator!() const { return !i; }

		inline bool testFlag(Enum f) const { return (i & f) == f && (f != 0 || i == int(f) ); }
	};
}