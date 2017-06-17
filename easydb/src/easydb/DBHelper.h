#ifndef _EASYDB_DB_HELPER_H_
#define _EASYDB_DB_HELPER_H_

#include <string>

namespace edb
{

class Database;
class LeafNode;

class DBHelper
{
public:
	static bool IsTreeClosure(const Database& db, const LeafNode* root);

	static void CopyTree(const Database& db, const LeafNode* root, const std::string& dst_dir);
	static void DeleteTree(const Database& db, int root);
	
}; // DBHelper

}

#endif // _EASYDB_DB_HELPER_H_