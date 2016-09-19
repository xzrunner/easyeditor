#ifndef _EASYSPRPACKER_NODE_BUILDER_H_
#define _EASYSPRPACKER_NODE_BUILDER_H_

#include "PackNode.h"

#include <ee/Visitor.h>

#include <CU_Uncopyable.h>

namespace esprpacker
{

class NodeBuilder : private cu::Uncopyable
{
public:
	virtual ~NodeBuilder() {}
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const = 0;
	virtual void Clear() = 0;

}; // NodeBuilder

}

#endif // _EASYSPRPACKER_NODE_BUILDER_H_