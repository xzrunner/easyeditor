#pragma once

#include "bsn/NodeSprBase.h"

namespace mm { class LinearAllocator; }
namespace bs { class ImportStream; class ExportStream; }
namespace Json { class Value; }

namespace bsn
{

class NodeSpr
{
public:
	virtual ~NodeSpr() {}

	//
	// serialization
	//
	virtual size_t GetBinSize() const = 0;
	virtual void StoreToBin(bs::ExportStream& es) const = 0;
	virtual void StoreToJson(Json::Value& val) const = 0;

	//
	// deserialization
	//
	virtual void LoadFromBin(mm::LinearAllocator& alloc, bs::ImportStream& is) = 0;
	virtual void LoadFromJson(mm::LinearAllocator& alloc, const Json::Value& val) = 0;

	const NodeSprBase& GetBaseInfo() const { return m_base_info; }

protected:
	NodeSprBase m_base_info;

}; // NodeSpr

}
