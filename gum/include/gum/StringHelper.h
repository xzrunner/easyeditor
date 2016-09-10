#ifndef _GUM_STRING_HELPER_H_
#define _GUM_STRING_HELPER_H_

#include <vector>
#include <sstream>
#include <algorithm>
#include <string>
#include <cctype>
#include <fstream>
#include <stdint.h>

namespace gum
{

class StringHelper
{
public:
	template<typename T>
	static std::string ToString(const T& d) {
		std::stringstream ss;
		ss << d;
		return ss.str();
	}

	template<typename T>
	static void FromString(const std::string& str, T& ret) {
		std::stringstream ss(str);
		ss >> ret;
	}

	template<typename T>
	static T FromString(const std::string& str) {
		std::stringstream ss(str);
		T ret;
		ss >> ret;
		return ret;
	}

	static void ToLower(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), std::tolower);
	}

	static void ToUpper(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), std::toupper);
	}

	static std::string GetLine(std::ifstream& fin)
	{
		std::string line;
		while ((line.empty() || line == " ") && getline(fin, line))
			;
		return line;
	}

	static void Split(const std::string& src, const std::string& mid, 
		std::vector<std::string>& dst);

	static std::string ToUtf8(const std::string& str);

}; // StringHelper

}

#endif // _GUM_STRING_HELPER_H_
