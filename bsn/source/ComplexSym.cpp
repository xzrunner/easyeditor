#include "bsn/ComplexSym.h"
#include "bsn/NodeFactory.h"
#include "bsn/NodeSpr.h"

#include <bs/ImportStream.h>
#include <bs/ExportStream.h>
#include <bs/typedef.h>
#include <bs/Serializer.h>

#include <json/json.h>

namespace bsn
{

void ComplexSym::StoreToBin(uint8_t** data, size_t& length) const
{
	length = GetBinSize();
	*data = new uint8_t[length];
	bs::ExportStream es(*data, length);
	
	// type
	es.Write(static_cast<uint8_t>(NODE_COMPLEX));

	// scissor
	for (int i = 0; i < 4; ++i) {
		es.Write(static_cast<uint16_t>(m_scissor[i]));
	}

	// children
	es.Write(static_cast<uint16_t>(m_children_n));
	for (int i = 0; i < m_children_n; ++i) {
		m_children[i]->StoreToBin(es);
	}

	// actions
	es.Write(static_cast<uint16_t>(m_actions_n));
	for (int i = 0; i < m_actions_n; ++i) {
		m_actions[i].StoreToBin(es);
	}
}

void ComplexSym::StoreToJson(Json::Value& val) const
{
	// scissor
	val["xmin"] = m_scissor[0];
	val["ymin"] = m_scissor[1];
	val["xmax"] = m_scissor[2];
	val["ymax"] = m_scissor[3];

	// children
	for (int i = 0; i < m_children_n; ++i) {
		m_children[i]->StoreToJson(val["sprite"][i]);
	}
}

ComplexSym* ComplexSym::Create(mm::LinearAllocator& alloc, bs::ImportStream& is)
{
	// scissor
	int16_t scissor[4];
	for (int i = 0; i < 4; ++i) {
		scissor[i] = is.UInt16();
	}

	size_t children_n = is.UInt16();

	size_t sz = ALIGN_4BYTE(sizeof(ComplexSym) - sizeof(NodeSpr*)
		+ sizeof(NodeSpr*) * children_n);
	void* ptr = alloc.alloc<char>(sz);
	ComplexSym* sym = new (ptr) ComplexSym();

	// scissor
	for (int i = 0; i < 4; ++i) {
		sym->m_scissor[i] = scissor[i];
	}

	// children
	sym->m_children_n = static_cast<uint16_t>(children_n);
	for (size_t i = 0; i < children_n; ++i) {
		sym->m_children[i] = NodeFactory::CreateNodeSpr(alloc, is);
	}

	// actions
	uint16_t actions_n = is.UInt16();
	sym->m_actions_n = static_cast<uint16_t>(actions_n);
	sym->m_actions = static_cast<Action*>(alloc.alloc<char>(sizeof(Action) * actions_n));
	for (size_t i = 0; i < actions_n; ++i)
	{
		Action* dst = &sym->m_actions[i];
		dst->name = is.String(alloc);
		dst->idx = bs::unpack_array16(alloc, is, 1, dst->n);
	}

	return sym;
}

ComplexSym* ComplexSym::Create(mm::LinearAllocator& alloc, Json::Value& val)
{
	int children_n = val["sprite"].size();

	size_t sz = ALIGN_4BYTE(sizeof(ComplexSym) - sizeof(NodeSpr*) 
		+ sizeof(NodeSpr*) * children_n);
	void* ptr = alloc.alloc<char>(sz);
	ComplexSym* sym = new (ptr) ComplexSym();

	// scissor
	sym->m_scissor[0] = val["xmin"].asInt();
	sym->m_scissor[1] = val["ymin"].asInt();
	sym->m_scissor[2] = val["xmax"].asInt();
	sym->m_scissor[3] = val["ymax"].asInt();

	// children
	sym->m_children_n = children_n;
	for (int i = 0; i < children_n; ++i) {
		sym->m_children[i] = NodeFactory::CreateNodeSpr(alloc, val["sprite"][i]);
	}

	// actions
	// todo	
	sym->m_actions_n = 0;
	sym->m_actions = nullptr;

	return sym;
}

void ComplexSym::GetScissor(int16_t& xmin, int16_t& ymin, int16_t& xmax, int16_t& ymax) const
{
	xmin = m_scissor[0];
	ymin = m_scissor[1];
	xmax = m_scissor[2];
	ymax = m_scissor[3];
}

size_t ComplexSym::GetBinSize() const
{
	size_t sz = 0;

	// type
	sz += sizeof(uint8_t);
	// scissor
	sz += sizeof(uint16_t) * 4;
	// children
	sz += sizeof(uint16_t);
	sz += sizeof(uint32_t) * m_children_n;
	for (int i = 0; i < m_children_n; ++i) {
		sz += m_children[i]->GetBinSize();
	}
	// actions
	sz += sizeof(uint16_t);
	for (int i = 0; i < m_actions_n; ++i) {
		sz += m_actions[i].GetBinSize();
	}

	return sz;
}

size_t ComplexSym::Action::GetBinSize() const
{
	size_t sz = 0;
	// todo
	sz += bs::pack_size(name);
	sz += sizeof(uint16_t);
	sz += sizeof(uint16_t) * n;
	return sz;
}

void ComplexSym::Action::StoreToBin(bs::ExportStream& es) const
{
	es.Write(name);
	es.Write(static_cast<uint16_t>(n));
	for (int i = 0; i < n; ++i) {
		es.Write(static_cast<uint16_t>(idx[i]));
	}
}

}