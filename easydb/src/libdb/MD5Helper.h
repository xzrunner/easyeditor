#ifndef _EASYDB_MD5_HELPER_H_
#define _EASYDB_MD5_HELPER_H_

#include <string>

namespace edb
{

class MD5Helper
{
public:
	static bool File(const std::string& filepath, std::string& sig);

}; // MD5Helper

}

#endif // _EASYDB_MD5_HELPER_H_