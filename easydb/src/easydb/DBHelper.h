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

	static void FixNodeRef(const Database& db, const LeafNode* removed);

	static void RenameNode(const Database& db, const LeafNode* node, const std::string& new_name);
	static void MoveNode(const Database& db, const LeafNode* node, const std::string& dst_dir);
	static void CopyNode(const Database& db, const LeafNode* node, const std::string& dst_dir);
	static void ChangeNode(const Database& db, const LeafNode* node, const std::string& new_node);

}; // DBHelper

}

#endif // _EASYDB_DB_HELPER_H_