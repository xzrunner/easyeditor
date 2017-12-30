#pragma once

#include "bsn/NodeType.h"

#include <string>

namespace mm { class LinearAllocator; }
namespace bs { class ImportStream; }
namespace Json { class Value;  }

namespace bsn
{

class NodeSpr;
class NodeSym;

class NodeFactory
{
public:
	static NodeSpr* CreateNodeSpr(mm::LinearAllocator& alloc, bs::ImportStream& is);
	static NodeSpr* CreateNodeSpr(mm::LinearAllocator& alloc, const Json::Value& val);

	static NodeSym* CreateNodeSym(mm::LinearAllocator& alloc, bs::ImportStream& is);
	static NodeSym* CreateNodeSym(mm::LinearAllocator& alloc, const std::string& filepath);

private:
	static NodeType GetNodeType(const std::string& filepath);

	static NodeSpr* CreateNodeSpr(mm::LinearAllocator& alloc, NodeType type);

}; // NodeFactory

}