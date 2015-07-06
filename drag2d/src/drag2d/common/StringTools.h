#ifndef _DRAG2D_STRING_TOOLS_H_
#define _DRAG2D_STRING_TOOLS_H_

#include <vector>
#include <sstream>
#include <algorithm>
#include <string>
#include <cctype>
#include <fstream>

namespace d2d
{

class StringTools
{
public:
	static std::string IntToString(int i)
	{
		std::stringstream ss;
		ss << i;
		return ss.str();
	}

	static int StringToInt(const std::string& str)
	{
		std::stringstream ss(str);
		int ret;
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

}; // StringTools

}

#endif // _DRAG2D_STRING_TOOLS_H_