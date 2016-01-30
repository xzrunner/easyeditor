#include "trans_color.h"

#include <string>

#include <assert.h>

namespace ee
{

Colorf TransColor(const std::string& str, PixelType type) 
{
	std::string snum = str;

	if (snum.empty())
		return Colorf(0, 0, 0, 0);

	if (snum == "0xffffffff")
		return Colorf(1, 1, 1, 1);

	if (snum[0] != '0' || (snum[1] != 'x' && snum[1] != 'X'))
	{
		int n = atoi(snum.c_str());
		char buffer[33];
		_itoa(n, buffer, 16);
		snum = "0x" + std::string(buffer);
	}

	int len = snum.length();

	Colorf ret(0, 0, 0, 1);
	if (len == 4)
	{
		if (type == PT_RGBA || PT_BGRA)
			ret.a = TransColor(snum[2], snum[3]);
		else if (type == PT_ARGB)
			ret.b = TransColor(snum[2], snum[3]);
		else if (type == PT_ABGR)
			ret.r = TransColor(snum[2], snum[3]);
	}
	else if (len == 10)
	{
		if (type == PT_RGBA) {
			ret.r = TransColor(snum[2], snum[3]);
			ret.g = TransColor(snum[4], snum[5]);
			ret.b = TransColor(snum[6], snum[7]);
			ret.a = TransColor(snum[8], snum[9]);
		} else if (type == PT_ARGB) {
			ret.a = TransColor(snum[2], snum[3]);
			ret.r = TransColor(snum[4], snum[5]);
			ret.g = TransColor(snum[6], snum[7]);
			ret.b = TransColor(snum[8], snum[9]);
		} else if (type == PT_ABGR) {
			ret.a = TransColor(snum[2], snum[3]);
			ret.b = TransColor(snum[4], snum[5]);
			ret.g = TransColor(snum[6], snum[7]);
			ret.r = TransColor(snum[8], snum[9]);
		}  else if (type == PT_BGRA) {
			ret.b = TransColor(snum[2], snum[3]);
			ret.g = TransColor(snum[4], snum[5]);
			ret.r = TransColor(snum[6], snum[7]);
			ret.a = TransColor(snum[8], snum[9]);
		}
	}

	return ret;
}

Colorf TransColor(int color, PixelType type)
{
	Colorf ret(0, 0, 0, 1);
	if (type == PT_RGBA)
	{
		ret.r = (color >> 24 & 0xff) / 255.0f;
		ret.g = (color >> 16 & 0xff) / 255.0f;
		ret.b = (color >> 8 & 0xff) / 255.0f;
		ret.a = (color & 0xff) / 255.0f;
	}
	else if (type == PT_ARGB)
	{
		ret.a = (color >> 24 & 0xff) / 255.0f;
		ret.r = (color >> 16 & 0xff) / 255.0f;
		ret.g = (color >> 8 & 0xff) / 255.0f;
		ret.b = (color & 0xff) / 255.0f;
	}
	else if (type == PT_ABGR)
	{
		ret.a = (color >> 24 & 0xff) / 255.0f;
		ret.b = (color >> 16 & 0xff) / 255.0f;
		ret.g = (color >> 8 & 0xff) / 255.0f;
		ret.r = (color & 0xff) / 255.0f;
	}
	else if (type == PT_BGRA)
	{
		ret.b = (color >> 24 & 0xff) / 255.0f;
		ret.g = (color >> 16 & 0xff) / 255.0f;
		ret.r = (color >> 8 & 0xff) / 255.0f;
		ret.a = (color & 0xff) / 255.0f;
	}
	return ret;
}

float TransColor(char high, char low) 
{
	int col = TransHex(high) * 16 + TransHex(low);
	assert(col >= 0 && col <= 255);
	return col / 255.0f;
}

int TransHex(char c) 
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

std::string TransColor(const Colorf& col, PixelType type) 
{
	std::string ret = "0x";
	if (type == PT_RGBA) {
		ret += TransColor(col.r);
		ret += TransColor(col.g);
		ret += TransColor(col.b);
		ret += TransColor(col.a);
	} else if (type == PT_ARGB) {
		ret += TransColor(col.a);
		ret += TransColor(col.r);
		ret += TransColor(col.g);
		ret += TransColor(col.b);
	} else if (type == PT_ABGR) {
		ret += TransColor(col.a);
		ret += TransColor(col.b);
		ret += TransColor(col.g);
		ret += TransColor(col.r);
	} else if (type == PT_BGRA) {
		ret += TransColor(col.b);
		ret += TransColor(col.g);
		ret += TransColor(col.r);
		ret += TransColor(col.a);
	}
	return ret;
}

std::string TransColor(float col) 
{
	assert(col >= 0 && col <= 1.0f);

	int c = (int)(col * 255 + 0.5f);
	int high = c / 16;
	int low = c % 16;

	std::string ret;
	ret += TransHex(high);
	ret += TransHex(low);

	return ret;
}

char TransHex(int v) 
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

uint32_t color2int(const Colorf& col, PixelType type)
{
	uint32_t ret = 0;
	if (type == PT_RGBA) {
		ret = (color2int(col.r)) << 24 
			| (color2int(col.g)) << 16
			| (color2int(col.b)) << 8
			| (color2int(col.a));
	} else if (type == PT_ARGB) {
		ret = (color2int(col.a)) << 24 
			| (color2int(col.r)) << 16
			| (color2int(col.g)) << 8
			| (color2int(col.b));
	} else if (type == PT_ABGR) {
		ret = (color2int(col.a)) << 24 
			| (color2int(col.b)) << 16
			| (color2int(col.g)) << 8
			| (color2int(col.r));
	} else if (type == PT_BGRA) {
		ret = (color2int(col.b)) << 24 
			| (color2int(col.g)) << 16
			| (color2int(col.r)) << 8
			| (color2int(col.a));
	}
	return ret;
}

uint32_t color2int(float col)
{
	assert(col >= 0 && col <= 1.0f);

	uint32_t c = ((uint32_t)(col * 255 + 0.5f) & 0xff);
	return c;
}

}