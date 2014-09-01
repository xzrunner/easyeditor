#include "color_trans.h"

#include <string>
#include <assert.h>

namespace d2d
{
Colorf transColor(const std::string& str, PixelType type) 
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
			ret.a = transColor(snum[2], snum[3]);
		else if (type == PT_ARGB)
			ret.b = transColor(snum[2], snum[3]);
	}
	else if (len == 10)
	{
		if (type == PT_RGBA) {
			ret.r = transColor(snum[2], snum[3]);
			ret.g = transColor(snum[4], snum[5]);
			ret.b = transColor(snum[6], snum[7]);
			ret.a = transColor(snum[8], snum[9]);
		} else if (type == PT_ARGB) {
			ret.a = transColor(snum[2], snum[3]);
			ret.r = transColor(snum[4], snum[5]);
			ret.g = transColor(snum[6], snum[7]);
			ret.b = transColor(snum[8], snum[9]);
		} else if (type == PT_BGRA) {
			ret.b = transColor(snum[2], snum[3]);
			ret.g = transColor(snum[4], snum[5]);
			ret.r = transColor(snum[6], snum[7]);
			ret.a = transColor(snum[8], snum[9]);
		}
	}

	return ret;
}

Colorf transColor(int color, PixelType type)
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
	else if (type == PT_BGRA)
	{
		ret.b = (color >> 24 & 0xff) / 255.0f;
		ret.g = (color >> 16 & 0xff) / 255.0f;
		ret.r = (color >> 8 & 0xff) / 255.0f;
		ret.a = (color & 0xff) / 255.0f;
	}
	return ret;
}

float transColor(char high, char low) 
{
	int col = transHex(high) * 16 + transHex(low);
	assert(col >= 0 && col <= 255);
	return col / 255.0f;
}

int transHex(char c) 
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

std::string transColor(const Colorf& col, PixelType type) 
{
	std::string ret = "0x";
	if (type == PT_RGBA) {
		ret += transColor(col.r);
		ret += transColor(col.g);
		ret += transColor(col.b);
		ret += transColor(col.a);
	} else if (type == PT_ARGB) {
		ret += transColor(col.a);
		ret += transColor(col.r);
		ret += transColor(col.g);
		ret += transColor(col.b);
	} else if (type == PT_BGRA) {
		ret += transColor(col.b);
		ret += transColor(col.g);
		ret += transColor(col.r);
		ret += transColor(col.a);
	}
	return ret;
}

std::string transColor(float col) 
{
	assert(col >= 0 && col <= 1.0f);

	int c = (int)(col * 255 + 0.5f);
	int high = c / 16;
	int low = c % 16;

	std::string ret;
	ret += transHex(high);
	ret += transHex(low);

	return ret;
}

char transHex(int v) 
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

int trans_color2int(const Colorf& col, PixelType type)
{
	int ret = 0;
	if (type == PT_RGBA) {
		ret = (trans_color2int(col.a)) << 24 
			| (trans_color2int(col.b)) << 16
			| (trans_color2int(col.g)) << 8
			| (trans_color2int(col.r));
	} else if (type == PT_ARGB) {
		ret = (trans_color2int(col.b)) << 24 
			| (trans_color2int(col.g)) << 16
			| (trans_color2int(col.r)) << 8
			| (trans_color2int(col.a));
	} else if (type == PT_BGRA) {
		ret = (trans_color2int(col.a)) << 24 
			| (trans_color2int(col.r)) << 16
			| (trans_color2int(col.g)) << 8
			| (trans_color2int(col.b));
	}
	return ret;
}

int trans_color2int(float col)
{
	assert(col >= 0 && col <= 1.0f);

	int c = (int)(col * 255 + 0.5f);
	return c;
}

}