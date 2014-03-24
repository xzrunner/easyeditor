#ifndef _EASYDB_SNAPSHOOT_H_
#define _EASYDB_SNAPSHOOT_H_

#include <drag2d.h>

namespace edb
{

class Snapshoot
{
public:
	static void trigger(const std::string& srcdir, const std::string& dstdir);

}; // Snapshoot

}

#endif // _EASYDB_SNAPSHOOT_H_