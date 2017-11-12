#ifndef _EASYDB_INDEX_NODE_H_
#define _EASYDB_INDEX_NODE_H_

#include "Node.h"
#include "NodeType.h"

#include <vector>

namespace edb
{

class IndexNode : public Node
{
public:
	IndexNode();
	IndexNode(const std::string& path);

	virtual int Type() const override { return NODE_INDEX; }

	virtual void Store(std::ofstream& fout) const override;
	virtual void Load(std::ifstream& fin) override;

	void SetChildren(const std::vector<int>& children) { m_children = children; }
	const std::vector<int>& GetChildren() const { return m_children; }
	
private:
	std::vector<int> m_children;

}; // IndexNode

}

#endif // _EASYDB_INDEX_NODE_H_