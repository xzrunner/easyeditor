#pragma once

#include "bsn/NodeType.h"

#include <string>

namespace mm { class LinearAllocator; }
namespace bs { class ImportStream; }
namespace Json { class Value;  }

namespace bsn
{

class NodeSpr;

class NodeFactory
{
public:
	static NodeSpr* CreateNodeSpr(mm::LinearAllocator& alloc, bs::ImportStream& is);
	static NodeSpr* CreateNodeSpr(mm::LinearAllocator& alloc, const Json::Value& val);

	

private:
	static NodeType GetNodeType(const std::string& filepath);

}; // NodeFactory

}