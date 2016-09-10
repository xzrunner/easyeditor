#ifndef _GUM_FILEPATH_HELPER_H_
#define _GUM_FILEPATH_HELPER_H_

#include <string>

namespace gum
{

class FilepathHelper
{
public:
	static std::string Relative(const std::string& base, const std::string& path);
	static std::string Absolute(const std::string& base, const std::string& path);

	static std::string Dir(const std::string& path);

	static std::string Format(const std::string& path);

	//////////////////////////////////////////////////////////////////////////

	static bool Exists(const std::string& path);

}; // FilepathHelper

}

#endif // _GUM_FILEPATH_HELPER_H_
