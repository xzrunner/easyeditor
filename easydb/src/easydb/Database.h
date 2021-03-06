#ifndef _EASYDB_DATABASE_H_
#define _EASYDB_DATABASE_H_

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
	const std::string& GetProjPath() const { return m_proj_path; }

	void Build(const std::string& path);

	int GetRoot() const { return m_root; }
	const std::vector<Node*>& GetNodes() const { return m_nodes; }

	int QueryByPath(const std::string& path) const;
	void QueryByStr(const std::string& str, int max, std::vector<int>& result) const;

	const Node* Fetch(int idx) const;

	const std::multimap<std::string, int>& GetMD5Map() const { return m_map_md5; }

	void RenamePath(const std::string& old_path, const std::string& new_path);

private:
	void Clear();

	int BuildNode(const std::string& path);

	void Insert(Node* node, int id);

	void FinalParse();

private:
	std::string m_proj_path;

	std::string m_dir_path;

	int m_root;
	
	std::vector<Node*> m_nodes;

	std::map<std::string, int> m_map_path;
	std::multimap<std::string, int> m_map_export_name;

	std::multimap<std::string, int> m_map_md5;

}; // Database

}

#endif // _EASYDB_DATABASE_H_