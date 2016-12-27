#include "PackNode.h"
#include "PackIDMgr.h"

#include <simp/NodeFactory.h>

namespace esprpacker
{

PackNode::PackNode()
	: m_node_id(-1)
	, m_pkg_id(-1)
{
}

int PackNode::GetID() const
{	
	return simp::NodeID::ComposeID(m_pkg_id, m_node_id);
}

void PackNode::SetFilepath(const std::string& filepath) const 
{
	m_filepath = filepath;
}

void PackNode::SetID(const std::string& filepath) const
{
	PackIDMgr::Instance()->QueryID(filepath, m_pkg_id, m_node_id);
}

}