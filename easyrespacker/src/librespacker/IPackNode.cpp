#include "IPackNode.h"

namespace librespacker
{

IPackNode::IPackNode() 
	: m_pkg_id(0xffff) 
{
	static int SPR_ID = 0;
	m_spr_id = SPR_ID++;
}

IPackNode::IPackNode(int spr_id) 
	: m_pkg_id(0xffff) 
{
	m_spr_id = spr_id;
}

}