#ifndef _TEST_DTEX_UTILITY_H_
#define _TEST_DTEX_UTILITY_H_

#include <easyejoy2d.h>

namespace tdtex
{

class Utility
{
public:
	static dtex_package* LoadPackage(const std::string& pkg_name, float scale = 1.0f,
		bool load_texture_immediately = false, int lod = 0/*, bool load_texture_async = false*/);

}; // Utility

}

#endif // _TEST_DTEX_UTILITY_H_