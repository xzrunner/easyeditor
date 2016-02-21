#ifndef _EASYEDITOR_STRING_HELPER_H_
#define _EASYEDITOR_STRING_HELPER_H_

#include <vector>
#include <string>

namespace ee
{

class StringHelper
{
public:
	template<typename T>
	static std::string ToString(const T& d);

	template<typename T>
	static void FromString(const std::string& str, T& ret);

	template<typename T>
	static T FromString(const std::string& str);

	static void ToLower(std::string& str);

	static void ToUpper(std::string& str);

	static std::string GetLine(std::ifstream& fin);

	static void Split(const std::string& src, const std::string& mid, 
		std::vector<std::string>& dst);

	static std::string ToUtf8(const std::string& str);

	static bool Replace(std::string& str, const std::string& from, const std::string& to);
	static void ReplaceAll(std::string& str, const std::string& from, const std::string& to);

	static std::string Format(const std::string str, ...);

}; // StringHelper

}

#include "StringHelper.inl"

#endif // _EASYEDITOR_STRING_HELPER_H_