#include "bsn/ColorParser.h"

#include <guard/check.h>

namespace bsn
{

static inline 
int char2hex(char c)
{
	if (c >= '0' && c <= '9') {
		return c - '0';
	} else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	} else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	} else {
		GD_REPORT_ASSERT("char2hex");
		return 0;
	}
}

static inline 
int char2channel(char high, char low)
{
	int col = char2hex(high) * 16 + char2hex(low);
	GD_ASSERT(col >= 0 && col <= 255, "char2channel");
	return col;
}

uint32_t ColorParser::StringToRGBA(const std::string& str, PIXEL_TYPE type)
{
	if (str.empty()) {
		return 0;
	}
	if (str == "0xffffffff") {
		return 0xffffffff;
	}

	std::string snum = str;
	if (snum[0] != '0' || (snum[1] != 'x' && snum[1] != 'X'))
	{
		int n = atoi(snum.c_str());
		char buffer[33];
		itoa(n, buffer, 16);
		snum = "0x" + std::string(buffer);
	}

	int len = snum.length();

	uint8_t r, g, b, a;
	if (len == 4)
	{
		if (type == RGBA || BGRA)
			a = char2channel(snum[2], snum[3]);
		else if (type == ARGB)
			b = char2channel(snum[2], snum[3]);
		else if (type == ABGR)
			r = char2channel(snum[2], snum[3]);
	}
	else if (len == 10)
	{
		if (type == RGBA) {
			r = char2channel(snum[2], snum[3]);
			g = char2channel(snum[4], snum[5]);
			b = char2channel(snum[6], snum[7]);
			a = char2channel(snum[8], snum[9]);
		} else if (type == ARGB) {
			a = char2channel(snum[2], snum[3]);
			r = char2channel(snum[4], snum[5]);
			g = char2channel(snum[6], snum[7]);
			b = char2channel(snum[8], snum[9]);
		} else if (type == ABGR) {
			a = char2channel(snum[2], snum[3]);
			b = char2channel(snum[4], snum[5]);
			g = char2channel(snum[6], snum[7]);
			r = char2channel(snum[8], snum[9]);
		}  else if (type == BGRA) {
			b = char2channel(snum[2], snum[3]);
			g = char2channel(snum[4], snum[5]);
			r = char2channel(snum[6], snum[7]);
			a = char2channel(snum[8], snum[9]);
		}
	}

	return r << 24 | g << 16 | b << 8 | a;
}

}