#pragma once

namespace mm { class LinearAllocator; }
namespace bs { class ImportStream; }

namespace bsn
{

class NodeSpr;

class NodeFactory
{
public:
	static NodeSpr* CreateNodeSpr(mm::LinearAllocator& alloc, bs::ImportStream& is);

}; // NodeFactory

}