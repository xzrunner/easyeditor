#include "color_trans.h"

#include <string>
#include <assert.h>

namespace d2d
{
Colorf transColor(const std::string& str) 
{
	if (str.empty())
		return Colorf(0, 0, 0, 0);

//	assert(str.length() == 10);
	if (str == "0xffffffff")
		return Colorf(1, 1, 1, 1);

	int len = str.length();

	Colorf ret;
	if (len < 4)
	{
		ret.r = ret.g = ret.b = ret.a = 0;
	}
	else if (len == 4)
	{
		ret.a = transColor(str[2], str[3]);
		ret.g = 0;
		ret.b = 0;
		ret.r = 0;
	}
	else if (len == 10)
	{
		ret.a = transColor(str[2], str[3]);
		ret.g = transColor(str[4], str[5]);
		ret.b = transColor(str[6], str[7]);
		ret.r = transColor(str[8], str[9]);
	}

// 	ret.a = (len >= 4 ? transColor(str[2], str[3]) : 0);
// 	ret.b = (len >= 6 ? transColor(str[4], str[5]) : 0);
// 	ret.g = (len >= 8 ? transColor(str[6], str[7]) : 0);
// 	ret.r = (len >= 10 ? transColor(str[8], str[9]) : 0);

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

std::string transColor(const Colorf& col) 
{
	std::string ret = "0x";
	ret += transColor(col.a);
	ret += transColor(col.g);
	ret += transColor(col.b);
	ret += transColor(col.r);
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
}