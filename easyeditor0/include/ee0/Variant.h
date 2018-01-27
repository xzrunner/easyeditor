#pragma once

#include "ee0/VariantType.h"

#include <stdint.h>

namespace ee0
{

class Variant
{
public:
	Variant() : m_type(VT_EMPTY) {}

	VariantType m_type;

	union
	{
		int8_t   b;              // VT_BYTE
		int16_t  i;              // VT_SHORT
		int32_t  l;              // VT_LONG
		int64_t  ll;             // VT_LONGLONG
		float    flt;            // VT_FLOAT
		double   dbl;            // VT_DOUBLE
		char     c;              // VT_CHAR
		uint16_t ui;             // VT_USHORT
		uint32_t ul;             // VT_ULONG
		uint64_t ull;            // VT_ULONGLONG
		bool     bl;             // VT_BOOL
		char*    pc;             // VT_PCHAR
		void*    pv;             // VT_PVOID
	} m_val;

}; // Variant

}