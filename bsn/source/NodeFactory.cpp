#include "bsn/NodeFactory.h"
#include "bsn/NodeType.h"

#include "bsn/ComplexSpr.h"

#include <guard/check.h>
#include <bs/ImportStream.h>

namespace bsn
{

NodeSpr* NodeFactory::CreateNodeSpr(mm::LinearAllocator& alloc, bs::ImportStream& is)
{
	NodeSpr* spr = nullptr;

	uint8_t type = is.UInt8();
	switch (type)
	{
	case NODE_COMPLEX:
		spr = ComplexSpr::Create(alloc, is);
		break;
	default:
		GD_REPORT_ASSERT("known node type.");
	}
	
	return spr;
}

}