#include "StringHelper.h"

#include <Windows.h>
#include <wx/string.h>

namespace ee
{

void StringHelper::ToLower(std::string& str)
{
//	std::transform(str.begin(), str.end(), str.begin(), std::tolower);
	str = wxString(str).Lower();
}

void StringHelper::ToUpper(std::string& str)
{
//	std::transform(str.begin(), str.end(), str.begin(), std::toupper);
	str = wxString(str).Upper();
}

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

bool StringHelper::Replace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if(start_pos == std::string::npos) {
		return false;
	}
	str.replace(start_pos, from.length(), to);
	return true;
}

void StringHelper::ReplaceAll(std::string& str, const std::string& from, const std::string& to)
{
	if(from.empty()) {
		return;
	}
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

std::string StringHelper::Format(const std::string fmt, ...) 
{
	int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
	std::string str;
	va_list ap;
	while (1) {     // Maximum two passes on a POSIX system...
		str.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size) {  // Everything worked
			str.resize(n);
			return str;
		}
		if (n > -1)  // Needed size returned
			size = n + 1;   // For null char
		else
			size *= 2;      // Guess at a larger size (OS specific)
	}
	return str;
}

}