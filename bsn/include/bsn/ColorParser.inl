#pragma once

namespace bsn
{

inline 
uint32_t ColorParser::Trans(uint32_t src, PIXEL_TYPE src_type, PIXEL_TYPE dst_type)
{
	uint8_t r = 0, g = 0, b = 0, a = 0;
	switch (src_type)
	{
	case RGBA:
		r = (src >> 24) & 0xff;
		g = (src >> 16) & 0xff;
		b = (src >> 8)  & 0xff;
		a = src         & 0xff;
		break;
	case ARGB:
		a = (src >> 24) & 0xff;
		r = (src >> 16) & 0xff;
		g = (src >> 8)  & 0xff;
		b = src         & 0xff;
		break;
	case ABGR:
		a = (src >> 24) & 0xff;
		b = (src >> 16) & 0xff;
		g = (src >> 8)  & 0xff;
		r = src         & 0xff;
		break;
	case BGRA:
		b = (src >> 24) & 0xff;
		g = (src >> 16) & 0xff;
		r = (src >> 8)  & 0xff;
		a = src         & 0xff;
		break;
	}

	uint32_t dst = 0;
	switch (dst_type)
	{
	case RGBA:
		dst = (r << 24) | (g << 16) | (b << 8) | a;
		break;
	case ARGB:
		dst = (a << 24) | (r << 16) | (g << 8) | b;
		break;
	case ABGR:
		dst = (a << 24) | (b << 16) | (g << 8) | r;
		break;
	case BGRA:
		dst = (b << 24) | (g << 16) | (r << 8) | a;
		break;
	}
	return dst;	
}

}