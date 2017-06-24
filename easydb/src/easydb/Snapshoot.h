#ifndef _EASYDB_SNAPSHOOT_H_
#define _EASYDB_SNAPSHOOT_H_

#include <string>

namespace edb
{

class Database;

class Snapshoot
{
public:
	static void Build(const Database& db, const std::string& dir);

}; // Snapshoot

}

#endif // _EASYDB_SNAPSHOOT_H_