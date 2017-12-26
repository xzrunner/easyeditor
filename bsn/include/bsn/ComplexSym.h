#pragma once

#include "bsn.h"

#include <string>

namespace mm { class LinearAllocator; }
namespace bs { class ImportStream; }
namespace json { class Value; }

namespace bsn
{

class NodeComplex : public INode
{
public:
	
	size_t GetBinSize() const;
	void   StoreToBin(byte** data, size_t& length) const;

	void StoreToJson() const;

	static NodeComplex* Create(mm::LinearAllocator& alloc, bs::ImportStream& is);
	static NodeComplex* Create(mm::LinearAllocator& alloc, json::Value& val);
	
public:


}; // NodeComplex

}
