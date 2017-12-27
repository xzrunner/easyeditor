#pragma once

namespace mm { class LinearAllocator; }
namespace bs { class ImportStream; }

namespace bsn
{

class INode;

class NodeFactory
{
public:
	static INode* CreateNodeSpr(mm::LinearAllocator& alloc, bs::ImportStream& is);

}; // NodeFactory

}