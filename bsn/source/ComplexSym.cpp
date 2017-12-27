#include "bsn/ComplexSym.h"
#include "bsn/NodeFactory.h"

#include <bs/ImportStream.h>
#include <bs/typedef.h>
#include <bs/Serializer.h>

#include <json/json.h>

namespace bsn
{

size_t ComplexSym::GetBinSize() const
{
	return 0;
}

void ComplexSym::StoreToBin(uint8_t** data, size_t& length) const
{

}

void ComplexSym::StoreToJson(Json::Value& val) const
{

}

ComplexSym* ComplexSym::Create(mm::LinearAllocator& alloc, bs::ImportStream& is)
{
	size_t sz = TypeSize();
	void* ptr = alloc.alloc<char>(sz);
	ComplexSym* sym = new (ptr) ComplexSym();

	// scissor
	for (int i = 0; i < 4; ++i) {
		uint16_t s = is.UInt16();
		sym->m_scissor[i] = s;
	}

	// children
	size_t n = is.UInt16();
	sym->m_children_n = static_cast<uint16_t>(n);
	for (size_t i = 0; i < n; ++i) {
		sym->m_children[i] = NodeFactory::CreateNodeSpr(alloc, is);
	}

	// actions
	n = is.UInt16();
	sym->m_actions_n = static_cast<uint16_t>(n);
	sym->m_actions = static_cast<Action*>(alloc.alloc<char>(sizeof(Action) * n));
	for (size_t i = 0; i < n; ++i)
	{
		Action* dst = &sym->m_actions[i];
		dst->name = is.String(alloc);
		dst->idx = bs::unpack_array16(alloc, is, 1, dst->n);
	}

	return sym;
}

ComplexSym* ComplexSym::Create(mm::LinearAllocator& alloc, Json::Value& val)
{
	size_t sz = TypeSize();
	void* ptr = alloc.alloc<char>(sz);
	ComplexSym* sym = new (ptr) ComplexSym();

	// scissor
	sym->m_scissor[0] = val["xmin"].asFloat();
	sym->m_scissor[1] = val["ymin"].asFloat();
	sym->m_scissor[2] = val["xmax"].asFloat();
	sym->m_scissor[3] = val["ymax"].asFloat();

	// children
	
	
	return sym;
}

size_t ComplexSym::TypeSize()
{	
	return ALIGN_4BYTE(sizeof(ComplexSym) - sizeof(Action));
}

}