#include "PackNode.h"

namespace esprpacker
{

PackNode::PackNode()
{
	static int NODE_ID = 0;
	m_id = NODE_ID++;
}

}