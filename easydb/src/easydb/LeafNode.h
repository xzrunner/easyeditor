#ifndef _EASYDB_LEAF_NODE_H_
#define _EASYDB_LEAF_NODE_H_

#include "Node.h"
#include "NodeType.h"

#include <vector>

#include <stdint.h>

namespace edb
{

class LeafNode : public Node
{
public:
	LeafNode();
	LeafNode(const std::string& path);
	
	virtual int Type() const { return NODE_LEAF; }

	virtual void Store(std::ofstream& fout) const;
	virtual void Load(std::ifstream& fin);

private:
	uint32_t m_timestamp;

	std::vector<int> m_in_nodes, m_out_nodes;

}; // LeafNode

}

#endif // _EASYDB_LEAF_NODE_H_