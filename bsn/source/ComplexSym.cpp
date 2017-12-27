#include "bsn/ComplexSym.h"

#include <bs/ImportStream.h>
#include <bs/typedef.h>
#include <bs/Serializer.h>

namespace bsn
{

size_t ComplexSym::GetBinSize() const
{

}

void ComplexSym::StoreToBin(byte** data, size_t& length) const
{

}

void ComplexSym::StoreToJson(json::Value& val) const
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

	// actiions
	size_t n = is.UInt16();
	sym->m_actions_n = n;
	for (int i = 0; i < n; ++i)
	{
		Action* dst = &sym->m_actions[i];
		dst->name = is.String(alloc);
		dst->idx = bs::unpack_array16(alloc, is, 1, dst->n);
	}
		
	// children
	n = is.UInt16();
	sym->m_children_n = n;
	sym->m_children = static_cast<INode**>(alloc.alloc<char>(sizeof(INode*) * n));
	memset(sym->m_children, 0, sizeof(INode*) * n);
	for (int i = 0; i < n; ++i) {
		sym->m_children[i] = 
	}
}

ComplexSym* ComplexSym::Create(mm::LinearAllocator& alloc, json::Value& val)
{
	return nullptr;
}

size_t ComplexSym::TypeSize()
{	
	return ALIGN_4BYTE(sizeof(ComplexSym) - sizeof(Action));
}

}