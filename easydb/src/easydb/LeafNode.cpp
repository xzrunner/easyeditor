#include "LeafNode.h"
#include "Serializer.h"

#include <fstream>

namespace edb
{

LeafNode::LeafNode()
	: m_timestamp(0)
{
}

LeafNode::LeafNode(const std::string& path)
	: Node(path)
	, m_timestamp(0)
{
}

void LeafNode::Store(std::ofstream& fout) const
{
	Node::Store(fout);

	fout.write(reinterpret_cast<const char*>(&m_timestamp), sizeof(uint32_t));	

	Serializer::WriteArrayInt16(fout, m_in_nodes);
	Serializer::WriteArrayInt16(fout, m_out_nodes);
}

void LeafNode::Load(std::ifstream& fin)
{
	Node::Load(fin);

	fin.read(reinterpret_cast<char*>(&m_timestamp), sizeof(uint32_t));

	Serializer::ReadArrayInt16(fin, m_in_nodes);
	Serializer::ReadArrayInt16(fin, m_out_nodes);
}

}