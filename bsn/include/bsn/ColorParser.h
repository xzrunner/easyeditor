#pragma once

#include <string>

#include <stdint.h>

namespace bsn
{

// text		RGBA	old
// text		ARGB
// add		ABGR
// multi	BGRA
enum PIXEL_TYPE
{
	RGBA = 0,
	ARGB,
	ABGR,
	BGRA
};

class ColorParser
{
public:
	static uint32_t StringToRGBA(const std::string& str, PIXEL_TYPE type);

	static uint32_t Trans(uint32_t src, PIXEL_TYPE src_type, PIXEL_TYPE dst_type);

}; // ColorParser

}

#include "bsn/ColorParser.inl"