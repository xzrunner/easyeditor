#include "LeafNode.h"
#include "Serializer.h"
#include "Database.h"
#include "JsonResParser.h"

#include <gum/FilepathHelper.h>

#include <fstream>

#include <assert.h>

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

	Serializer::WriteStr(fout, m_export_name);

	Serializer::WriteSetInt16(fout, m_in_nodes);
	Serializer::WriteSetInt16(fout, m_out_nodes);
}

void LeafNode::Load(std::ifstream& fin)
{
	Node::Load(fin);

	fin.read(reinterpret_cast<char*>(&m_timestamp), sizeof(uint32_t));
	
	Serializer::ReadStr(fin, m_export_name);

	Serializer::ReadSetInt16(fin, m_in_nodes);
	Serializer::ReadSetInt16(fin, m_out_nodes);
}

void LeafNode::BuildConnection(const Database& db)
{
	std::string filepath = gum::FilepathHelper::Absolute(db.GetDirPath(), GetPath());
	JsonResParser::Parse(db, filepath, m_export_name, m_out_nodes);

	std::set<int>::iterator itr = m_out_nodes.begin();
	for ( ; itr != m_out_nodes.end(); ++itr) 
	{
		if (*itr == -1) {
			continue;
		}
		const Node* to = db.Fetch(*itr);
		assert(to && to->Type() == NODE_LEAF);
		const_cast<LeafNode*>(static_cast<const LeafNode*>(to))->AddInput(GetID());		
	}
}

void LeafNode::AddInput(int id)
{
	m_in_nodes.insert(id);
}

}