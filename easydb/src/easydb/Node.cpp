#include "Node.h"
#include "Serializer.h"

#include <gum/StringHelper.h>

namespace edb
{

Node::Node()
	: m_id(-1)
{
}

Node::Node(const std::string& path) 
	: m_path(path) 
	, m_id(-1)
{
	gum::StringHelper::ToLower(m_path);
}

Node::~Node() 
{
}

void Node::Store(std::ofstream& fout) const
{
	Serializer::WriteStr(fout, m_path);
}

void Node::Load(std::ifstream& fin)
{
	Serializer::ReadStr(fin, m_path);
}

}