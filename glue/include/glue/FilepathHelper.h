#ifndef _GLUE_FILEPATH_HELPER_H_
#define _GLUE_FILEPATH_HELPER_H_

#include <string>

namespace glue
{

class FilepathHelper
{
public:
	static std::string Relative(const std::string& base, const std::string& path);
	static std::string Absolute(const std::string& base, const std::string& path);

	static std::string Format(const std::string& path);

}; // FilepathHelper

}

#endif // _GLUE_FILEPATH_HELPER_H_