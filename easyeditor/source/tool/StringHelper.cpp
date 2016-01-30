#include "StringHelper.h"

#include <Windows.h>

namespace ee
{

void StringHelper::Split(const std::string& src, const std::string& mid, 
						std::vector<std::string>& dst)
{
	char* cstr = new char [src.length()+1];
	std::strcpy(cstr, src.c_str());
	char* p = std::strtok(cstr, mid.c_str());
	while (p!=0)
	{
		dst.push_back(std::string(p));
		p = strtok(NULL, mid.c_str());
	}
	delete[] cstr;
}

std::string StringHelper::ToUtf8(const std::string& str)
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

}