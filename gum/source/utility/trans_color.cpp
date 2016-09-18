#include "trans_color.h"

#include <assert.h>

namespace gum
{

static inline 
int char2hex(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	else {
		assert(0);
		return 0;
	}
}

static inline 
int char2channel(char high, char low)
{
	int col = char2hex(high) * 16 + char2hex(low);
	assert(col >= 0 && col <= 255);
	return col;
}

s2::Color str2color(const std::string& str, PixelType type)
{
	std::string snum = str;

	if (snum.empty()) {
		return s2::Color();
	}
	if (snum == "0xffffffff") {
		return s2::Color(0xffffffff);
	}

	if (snum[0] != '0' || (snum[1] != 'x' && snum[1] != 'X'))
	{
		int n = atoi(snum.c_str());
		char buffer[33];
		_itoa(n, buffer, 16);
		snum = "0x" + std::string(buffer);
	}

	int len = snum.length();

	s2::Color ret(0, 0, 0, 1);
	if (len == 4)
	{
		if (type == PT_RGBA || PT_BGRA)
			ret.a = char2channel(snum[2], snum[3]);
		else if (type == PT_ARGB)
			ret.b = char2channel(snum[2], snum[3]);
		else if (type == PT_ABGR)
			ret.r = char2channel(snum[2], snum[3]);
	}
	else if (len == 10)
	{
		if (type == PT_RGBA) {
			ret.r = char2channel(snum[2], snum[3]);
			ret.g = char2channel(snum[4], snum[5]);
			ret.b = char2channel(snum[6], snum[7]);
			ret.a = char2channel(snum[8], snum[9]);
		} else if (type == PT_ARGB) {
			ret.a = char2channel(snum[2], snum[3]);
			ret.r = char2channel(snum[4], snum[5]);
			ret.g = char2channel(snum[6], snum[7]);
			ret.b = char2channel(snum[8], snum[9]);
		} else if (type == PT_ABGR) {
			ret.a = char2channel(snum[2], snum[3]);
			ret.b = char2channel(snum[4], snum[5]);
			ret.g = char2channel(snum[6], snum[7]);
			ret.r = char2channel(snum[8], snum[9]);
		}  else if (type == PT_BGRA) {
			ret.b = char2channel(snum[2], snum[3]);
			ret.g = char2channel(snum[4], snum[5]);
			ret.r = char2channel(snum[6], snum[7]);
			ret.a = char2channel(snum[8], snum[9]);
		}
	}

	return ret;
}

static inline
char hex2char(int v) 
{
	assert(v >= 0 && v <= 15);
	if (v >= 0 && v <= 9)
		return '0' + v;
	else if (v >= 10 && v <= 15)
		return 'a' + (v - 10);
	else {
		assert(0);
		return '0';
	}
}

static inline
std::string channel2char(int col) 
{
	assert(col >= 0 && col <= 255);

	int high = col / 16,
		low  = col % 16;

	std::string ret;
	ret += hex2char(high);
	ret += hex2char(low);

	return ret;
}

std::string color2str(const s2::Color& col, PixelType type)
{
	std::string ret = "0x";
	if (type == PT_RGBA) {
		ret += channel2char(col.r);
		ret += channel2char(col.g);
		ret += channel2char(col.b);
		ret += channel2char(col.a);
	} else if (type == PT_ARGB) {
		ret += channel2char(col.a);
		ret += channel2char(col.r);
		ret += channel2char(col.g);
		ret += channel2char(col.b);
	} else if (type == PT_ABGR) {
		ret += channel2char(col.a);
		ret += channel2char(col.b);
		ret += channel2char(col.g);
		ret += channel2char(col.r);
	} else if (type == PT_BGRA) {
		ret += channel2char(col.b);
		ret += channel2char(col.g);
		ret += channel2char(col.r);
		ret += channel2char(col.a);
	}
	return ret;
}

s2::Color int2color(uint32_t i, PixelType type)
{
	s2::Color col;
	switch (type)
	{
	case PT_RGBA:
		col.r = (i >> 24) & 0xff;
		col.g = (i >> 16) & 0xff;
		col.b = (i >> 8)  & 0xff;
		col.a = i         & 0xff;
		break;
	case PT_ARGB:
		col.a = (i >> 24) & 0xff;
		col.r = (i >> 16) & 0xff;
		col.g = (i >> 8)  & 0xff;
		col.b = i         & 0xff;
		break;
	case PT_ABGR:
		col.a = (i >> 24) & 0xff;
		col.b = (i >> 16) & 0xff;
		col.g = (i >> 8)  & 0xff;
		col.r = i         & 0xff;
		break;
	case PT_BGRA:
		col.b = (i >> 24) & 0xff;
		col.g = (i >> 16) & 0xff;
		col.r = (i >> 8)  & 0xff;
		col.a = i         & 0xff;
		break;
	}
	return col;
}

uint32_t color2int(const s2::Color& col, PixelType type)
{
	uint32_t i = 0;
	switch (type)
	{
	case PT_RGBA:
		i  = (col.r << 24 ) | (col.g << 16) | (col.b << 8) | col.a;
		break;
	case PT_ARGB:
		i  = (col.a << 24 ) | (col.r << 16) | (col.g << 8) | col.b;
		break;
	case PT_ABGR:
		i  = (col.a << 24 ) | (col.b << 16) | (col.g << 8) | col.r;
		break;
	case PT_BGRA:
		i  = (col.b << 24 ) | (col.g << 16) | (col.r << 8) | col.a;
		break;
	}
	return i;
}

uint32_t color2int(const uint8_t rgba[4], PixelType type)
{
	uint32_t i = 0;
	switch (type)
	{
	case PT_RGBA:
		i  = (rgba[0] << 24 ) | (rgba[1] << 16) | (rgba[2] << 8) | rgba[3];
		break;
	case PT_ARGB:
		i  = (rgba[3] << 24 ) | (rgba[0] << 16) | (rgba[1] << 8) | rgba[2];
		break;
	case PT_ABGR:
		i  = (rgba[3] << 24 ) | (rgba[2] << 16) | (rgba[1] << 8) | rgba[0];
		break;
	case PT_BGRA:
		i  = (rgba[2] << 24 ) | (rgba[1] << 16) | (rgba[0] << 8) | rgba[3];
		break;
	}
	return i;
}

}