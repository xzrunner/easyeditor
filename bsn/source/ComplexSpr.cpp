#include "bsn/ComplexSpr.h"

#include <bs/ImportStream.h>

namespace bsn
{

ComplexSpr::ComplexSpr()
	: m_action(0)
{
}

size_t ComplexSpr::GetBinSize() const
{
	return 0;
}

void ComplexSpr::StoreToBin(byte** data, size_t& length) const
{

}

void ComplexSpr::StoreToJson(json::Value& val) const
{

}

ComplexSpr* ComplexSpr::Create(mm::LinearAllocator& alloc, bs::ImportStream& is)
{
	void* ptr = alloc.alloc<char>(sizeof(ComplexSpr));
	ComplexSpr* spr = new (ptr) ComplexSpr();
	spr->LoadFromBin(alloc, is);
	return spr;
}

ComplexSpr* ComplexSpr::Create(mm::LinearAllocator& alloc, const json::Value& val)
{
	return nullptr;
}

void ComplexSpr::LoadFromBin(mm::LinearAllocator& alloc, bs::ImportStream& is)
{
	NodeSpr::LoadFromBin(alloc, is);
	m_action = is.UInt16();
}

void ComplexSpr::LoadFromJson(mm::LinearAllocator& alloc, const json::Value& val)
{
	
}

}