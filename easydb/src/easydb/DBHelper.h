#ifndef _EASYDB_DB_HELPER_H_
#define _EASYDB_DB_HELPER_H_

namespace edb
{

class Database;
class LeafNode;

class DBHelper
{
public:
	static bool IsTreeClosure(const Database& db, const LeafNode* root);
	
}; // DBHelper

}

#endif // _EASYDB_DB_HELPER_H_