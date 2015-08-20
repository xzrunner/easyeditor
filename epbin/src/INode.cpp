#include "INode.h"

namespace epbin
{

size_t INode::Size() const
{
	return sizeof(m_type);
}

void INode::Store(uint8_t** ptr)
{
	memcpy(*ptr, &m_type, sizeof(m_type));
	*ptr += sizeof(m_type);
}


}