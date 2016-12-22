#ifndef _EASYEDITOR_STRING_HELPER_INL_
#define _EASYEDITOR_STRING_HELPER_INL_

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

namespace ee
{

template<typename T>
std::string StringHelper::ToString(const T& d) 
{
	std::stringstream ss;
	ss << d;
	return ss.str();
}

template<typename T>
void StringHelper::FromString(const std::string& str, T& ret) 
{
	std::stringstream ss(str);
	ss >> ret;
}

template<typename T>
T StringHelper::FromString(const std::string& str) 
{
	std::stringstream ss(str);
	T ret;
	ss >> ret;
	return ret;
}

inline
std::string StringHelper::GetLine(std::ifstream& fin)
{
	std::string line;
	while ((line.empty() || line == " ") && getline(fin, line))
		;
	return line;
}

}

#endif // _EASYEDITOR_STRING_HELPER_INL_
