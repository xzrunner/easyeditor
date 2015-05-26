#ifndef _DRAG2D_STRING_TOOLS_H_
#define _DRAG2D_STRING_TOOLS_H_

#include <vector>

namespace d2d
{

class StringTools
{
public:
	static void Split(const std::string& src, const std::string& mid, 
		std::vector<std::string>& dst);

}; // StringTools

}

#endif // _DRAG2D_STRING_TOOLS_H_