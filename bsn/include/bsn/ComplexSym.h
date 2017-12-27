#pragma once

#include "bsn.h"

#include <memmgr/LinearAllocator.h>

#include <string>
#include <memory>

namespace mm { class LinearAllocator; }
namespace bs { class ImportStream; }
namespace json { class Value; }

namespace bsn
{

class ComplexSym : public INode
{
public:
	virtual size_t GetBinSize() const override;
	virtual void StoreToBin(byte** data, size_t& length) const override;
	virtual void StoreToJson(json::Value& val) const override;

	static ComplexSym* Create(mm::LinearAllocator& alloc, bs::ImportStream& is);
	static ComplexSym* Create(mm::LinearAllocator& alloc, json::Value& val);
	
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
	uint16_t m_children_n;
	uint16_t m_actions_n;

	int16_t m_scissor[4];

	INode** m_children;
	
	Action m_actions[1];
	
}; // ComplexSym

}
