#ifndef _TEST_DTEX_UTILITY_H_
#define _TEST_DTEX_UTILITY_H_

#include <easyejoy2d.h>

namespace tdtex
{

class Utility
{
public:
	static dtex_package* LoadPackage(const std::string& pkg, bool load_tex = false);

}; // Utility

}

#endif // _TEST_DTEX_UTILITY_H_