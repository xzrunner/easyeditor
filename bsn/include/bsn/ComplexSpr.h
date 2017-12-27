#pragma once

#include "bsn/NodeSpr.h"

namespace bsn
{

class ComplexSpr : public NodeSpr
{
public:
	virtual size_t GetBinSize() const override;
	virtual void StoreToBin(byte** data, size_t& length) const override;
	virtual void StoreToJson(json::Value& val) const override;

	static ComplexSpr* Create(mm::LinearAllocator& alloc, bs::ImportStream& is);
	static ComplexSpr* Create(mm::LinearAllocator& alloc, const json::Value& val);

protected:
	virtual void LoadFromBin(mm::LinearAllocator& alloc, bs::ImportStream& is);
	virtual void LoadFromJson(mm::LinearAllocator& alloc, const json::Value& val);

private:
	ComplexSpr();

private:
	uint32_t m_action;

}; // ComplexSpr

}