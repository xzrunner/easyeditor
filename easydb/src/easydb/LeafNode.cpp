#include "LeafNode.h"
#include "Serializer.h"
#include "Database.h"
#include "JsonResParserOP.h"
#include "MD5Helper.h"

#include <gum/FilepathHelper.h>

#include <fstream>

#include <assert.h>
#include <sys/stat.h> 

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

	fout.write(reinterpret_cast<const char*>(&m_timestamp), sizeof(m_timestamp));

	Serializer::WriteStr(fout, m_md5);

	Serializer::WriteStr(fout, m_export_name);

	Serializer::WriteSetInt16(fout, m_in_nodes);
	Serializer::WriteSetInt16(fout, m_out_nodes);
}

void LeafNode::Load(std::ifstream& fin)
{
	Node::Load(fin);

	fin.read(reinterpret_cast<char*>(&m_timestamp), sizeof(m_timestamp));

	Serializer::ReadStr(fin, m_md5);

	Serializer::ReadStr(fin, m_export_name);

	Serializer::ReadSetInt16(fin, m_in_nodes);
	Serializer::ReadSetInt16(fin, m_out_nodes);
}

void LeafNode::Parser(const Database& db)
{
	std::string filepath = db.GetDirPath() + "\\" + GetPath();

	// export name and out nodes
	JsonResParserOP parser(filepath, db, m_export_name, m_out_nodes);
	parser.Do();

	// in nodes
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

	// timestamp
	struct stat st;
	stat(filepath.c_str(), &st);
	m_timestamp = st.st_mtime;

	// md5
	m_md5.resize(32);
	MD5Helper::File(filepath, m_md5);	
}

void LeafNode::AddInput(int id)
{
	m_in_nodes.insert(id);
}

void LeafNode::ClearInput()
{
	m_in_nodes.clear();
}

bool LeafNode::IsRefError() const
{
	return m_out_nodes.find(-1) != m_out_nodes.end();
}

bool LeafNode::IsNoUse() const
{
	return m_in_nodes.empty() && m_export_name.empty();
}

}