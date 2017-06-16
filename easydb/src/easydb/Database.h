#ifndef _EASYDB_DATABASE_H_
#define _EASYDB_DATABASE_H_

//#include <CU_Singleton.h>

#include <string>
#include <vector>
#include <map>

namespace edb
{

class Node;

class Database
{
public:	
	Database();
	~Database();

	void Store(const std::string& filepath) const;
	void Load(const std::string& filepath);

	const std::string& GetDirPath() const { return m_dir_path; }

	void Build(const std::string& path);

	int GetRoot() const { return m_root; }
	const std::vector<Node*>& GetNodes() const { return m_nodes; }

private:
	void Clear();

	int BuildNode(const std::string& path);

private:
	std::string m_dir_path;

	int m_root;
	
	std::vector<Node*> m_nodes;

	std::map<std::string, int> m_map;

//	SINGLETON_DECLARATION(Database);

}; // Database

}

#endif // _EASYDB_DATABASE_H_