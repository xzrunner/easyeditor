#ifndef _EASYRESPACKER_INODE_BUILDER_H_
#define _EASYRESPACKER_INODE_BUILDER_H_

#include "IPackNode.h"

#include <ee/Visitor.h>

namespace erespacker
{

class INodeBuilder
{
public:
	virtual ~INodeBuilder() {}
	virtual void Traverse(ee::Visitor<IPackNode>& visitor) const = 0;
}; // INodeBuilder

}

#endif // _EASYRESPACKER_INODE_BUILDER_H_