#include "IndexNode.h"
#include "Serializer.h"

namespace edb
{

IndexNode::IndexNode()
{
}

IndexNode::IndexNode(const std::string& path)
	: Node(path)
{
}

void IndexNode::Store(std::ofstream& fout) const
{
	Node::Store(fout);

	Serializer::WriteArrayInt16(fout, m_children);
}

void IndexNode::Load(std::ifstream& fin)
{
	Node::Load(fin);

	Serializer::ReadArrayInt16(fin, m_children);
}

}