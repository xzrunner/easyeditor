#include "text_util.h"

#include "common/tools.h"

#include <windows.h>

namespace d2d
{

std::string string2utf8(const std::string& str)
{
	int size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(),
		str.length(), NULL, 0);
	std::wstring utf16_str(size, '\0');
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(),
		str.length(), &utf16_str[0], size);

	int utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
		utf16_str.length(), NULL, 0,
		NULL, NULL);
	std::string utf8_str(utf8_size, '\0');
	WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
		utf16_str.length(), &utf8_str[0], utf8_size,
		NULL, NULL);
	return utf8_str;
}

int _copystr(char *utf8, const char *str, int n)
{
	int i;
	utf8[0] = str[0];
	int unicode = utf8[0] & ((1 << (8-n)) - 1);
	for (i=1;i<n;i++) {
		utf8[i] = str[i];
		unicode = unicode << 6 | (utf8[i] & 0x3f);
	}
	utf8[i] = 0;
	return unicode;
}

void utf8_to_unicode(const char* utf8_src, std::vector<int>& unicode_dst)
{
	char utf8[7];
	for (int i=0; utf8_src[i] ;) 
	{
		int unicode;
		uint8_t c = (uint8_t)utf8_src[i];
		if ((c&0x80) == 0) {
			unicode = _copystr(utf8,&utf8_src[i],1);
			i+=1;
		} else if ((c&0xe0) == 0xc0) {
			unicode = _copystr(utf8,&utf8_src[i],2);
			i+=2;
		} else if ((c&0xf0) == 0xe0) {
			unicode = _copystr(utf8,&utf8_src[i],3);
			i+=3;
		} else if ((c&0xf8) == 0xf0) {
			unicode = _copystr(utf8,&utf8_src[i],4);
			i+=4;
		} else if ((c&0xfc) == 0xf8) {
			unicode = _copystr(utf8,&utf8_src[i],5);
			i+=5;
		} else {
			unicode = _copystr(utf8,&utf8_src[i],6);
			i+=6;
		}

		unicode_dst.push_back(unicode);
	}	
}

}