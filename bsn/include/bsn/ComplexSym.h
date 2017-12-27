#pragma once

#include "bsn/NodeSym.h"

#include <memmgr/LinearAllocator.h>

#include <string>
#include <memory>

namespace mm { class LinearAllocator; }
namespace bs { class ImportStream; }

namespace bsn
{

class NodeSpr;

class ComplexSym : public NodeSym
{
public:
	//
	// serialization
	//
	virtual size_t GetBinSize() const override;
	virtual void StoreToBin(uint8_t** data, size_t& length) const override;
	virtual void StoreToJson(Json::Value& val) const override;

	//
	// deserialization
	//
	static ComplexSym* Create(mm::LinearAllocator& alloc, bs::ImportStream& is);
	static ComplexSym* Create(mm::LinearAllocator& alloc, Json::Value& val);
	
private:
	static size_t TypeSize();

public:
	struct Action
	{
		const char* name;
		uint16_t* idx;
		uint16_t n;
	};

private:
	int16_t m_scissor[4];

	uint16_t m_children_n;
	uint16_t m_actions_n;

	Action* m_actions;

	NodeSpr* m_children[1];

}; // ComplexSym

}
