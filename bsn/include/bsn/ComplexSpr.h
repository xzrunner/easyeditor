#pragma once

#include "bsn/NodeSpr.h"

namespace bsn
{

class ComplexSpr : public NodeSpr
{
public:
	//
	// serialization
	//
	virtual size_t GetBinSize() const override;
	virtual void StoreToBin(uint8_t** ptr) const override;
	virtual void StoreToJson(Json::Value& val) const override;

	//
	// deserialization
	//
	static ComplexSpr* Create(mm::LinearAllocator& alloc, bs::ImportStream& is);
	static ComplexSpr* Create(mm::LinearAllocator& alloc, const Json::Value& val);

protected:
	virtual void LoadFromBin(mm::LinearAllocator& alloc, bs::ImportStream& is);
	virtual void LoadFromJson(mm::LinearAllocator& alloc, const Json::Value& val);

private:
	ComplexSpr();

private:
	uint32_t m_action;

}; // ComplexSpr

}