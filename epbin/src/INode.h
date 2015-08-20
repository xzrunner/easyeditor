#ifndef _EPBIN_INODE_H_
#define _EPBIN_INODE_H_

#include <stdint.h>

namespace epbin
{

class INode
{
public:
	INode(uint8_t type) : m_type(type) {}

	virtual size_t Size() const;

	virtual void Store(uint8_t** ptr);

protected:
	uint8_t m_type;

}; // INode

}

#endif // _EPBIN_INODE_H_