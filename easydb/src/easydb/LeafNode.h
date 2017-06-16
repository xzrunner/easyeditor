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

	void BuildConnection(const Database& db);

	void AddInput(int id);

	const std::set<int>& GetNodes(bool in) const { 
		return in ? m_in_nodes : m_out_nodes; }

private:
	uint32_t m_timestamp;

	std::set<int> m_in_nodes, m_out_nodes;

}; // LeafNode

}

#endif // _EASYDB_LEAF_NODE_H_