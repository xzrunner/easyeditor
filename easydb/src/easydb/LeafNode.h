#ifndef _EASYDB_LEAF_NODE_H_
#define _EASYDB_LEAF_NODE_H_

#include "Node.h"
#include "NodeType.h"

#include <json/json.h>

#include <set>

#include <stdint.h>

namespace edb
{

class Database;

class LeafNode : public Node
{
public:
	LeafNode();
	LeafNode(const std::string& path);
	
	virtual int Type() const { return NODE_LEAF; }

	virtual void Store(std::ofstream& fout) const;
	virtual void Load(std::ifstream& fin);

	time_t GetTimestamp() const { return m_timestamp; }

	const std::string& GetMD5() const { return m_md5; }

	const std::string& GetExportName() const { return m_export_name; }

	void Parser(const Database& db);

	void AddInput(int id);

	const std::set<int>& GetNodes(bool in) const { 
		return in ? m_in_nodes : m_out_nodes; }

private:
	time_t m_timestamp;

	std::string m_md5;

	std::string m_export_name;
	
	std::set<int> m_in_nodes, m_out_nodes;

}; // LeafNode

}

#endif // _EASYDB_LEAF_NODE_H_