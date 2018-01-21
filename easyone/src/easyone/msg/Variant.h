#pragma once

#include "msg/VariantType.h"

#include <stdint.h>

namespace eone
{

class Variant
{
public:
	Variant();

	VariantType m_varType;

	union
	{
		int8_t  bVal;              // VT_BYTE
		int16_t iVal;              // VT_SHORT
		int32_t lVal;              // VT_LONG
		int64_t llVal;             // VT_LONGLONG
		float fltVal;              // VT_FLOAT
		double dblVal;             // VT_DOUBLE
		char cVal;                 // VT_CHAR
		uint16_t uiVal;            // VT_USHORT
		uint32_t ulVal;            // VT_ULONG
		uint64_t ullVal;           // VT_ULONGLONG
		bool blVal;                // VT_BOOL
		char* pcVal;               // VT_PCHAR
		void* pvVal;               // VT_PVOID
	} m_val;
}; // Variant

}