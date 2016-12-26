#ifndef _SHADERLAB_PARSER_STRING_HELPER_H_
#define _SHADERLAB_PARSER_STRING_HELPER_H_

#include <string>

namespace sl
{
namespace parser
{

class StringHelper
{
public:
	static void ReplaceAll(std::string& str, const std::string& from, const std::string& to);

}; // StringHelper

}
}

#endif // _SHADERLAB_PARSER_STRING_HELPER_H_