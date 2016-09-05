#include "PackComplex.h"
#include "PackNodeFactory.h"

#include <easycomplex.h>

#include <sprite2/S2_Sprite.h>

#include <simp_pack.h>

namespace esprpacker
{

PackComplex::PackComplex(const ecomplex::Symbol* sym)
{
	Init(sym);
}

PackComplex::~PackComplex()
{

}

void PackComplex::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{

}

int PackComplex::SizeOfUnpackFromBin() const
{
	int sz = SIZEOF_COMPLEX;
	sz += sizeof(uint32_t) * m_children.size();
	for (int i = 0, n = m_actions.size(); i < n; ++i) {
		sz += m_actions[i].SizeOfUnpackFromBin();
	}
	return sz;
}

int PackComplex::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint16_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint16_t) + sizeof(uint32_t) * m_children.size();		// children
	sz += sizeof(uint16_t);			// actions
	for (int i = 0, n = m_actions.size(); i < n; ++i) {
		sz += m_actions[i].SizeOfPackToBin();
	}
	return sz;
}

void PackComplex::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = TYPE_COMPLEX;
	pack(type, ptr);

	// children
	uint16_t children_n = m_children.size();
	pack(children_n, ptr);
	for (int i = 0; i < children_n; ++i) {
		uint32_t id = m_children[i]->GetID();
		pack(id, ptr);
	}

	// actions
	uint16_t action_n = m_actions.size();
	pack(action_n, ptr);
	for (int i = 0; i < action_n; ++i) {
		m_actions[i].PackToBin(ptr, m_children);
	}
}

void PackComplex::Init(const ecomplex::Symbol* sym)
{
	const std::vector<s2::Sprite*>& children = sym->GetChildren();
	m_children.reserve(children.size());
	for (int i = 0, n = children.size(); i < n; ++i) {
		m_children.push_back(PackNodeFactory::Instance()->Create(
			dynamic_cast<ee::Sprite*>(children[i])));
	}

	const std::vector<s2::ComplexSymbol::Action>& actions = sym->GetActions();
	for (int i = 0, n = actions.size(); i < n; ++i) {
		const s2::ComplexSymbol::Action& src = actions[i];
		Action dst;
		dst.m_name = src.name;
		dst.m_sprs.reserve(src.sprs.size());
		for (int j = 0, m = src.sprs.size(); j < m; ++j) {
			dst.m_sprs.push_back(PackNodeFactory::Instance()->Create(
				dynamic_cast<ee::Sprite*>(src.sprs[i])));
		}
		m_actions.push_back(dst);
	}

	m_scissor = sym->GetScissor();
}

/************************************************************************/
/* class PackComplex::Action                                                                     */
/************************************************************************/

int PackComplex::Action::
SizeOfUnpackFromBin() const 
{
	int sz = SIZEOF_COMPLEX_ACTION;
	sz += sizeof_unpack_str(m_name);
	sz += sizeof(uint16_t) * m_sprs.size();
	return sz;
}

int PackComplex::Action::
SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof_unpack_str(m_name);							// name
	sz += sizeof(uint16_t) + m_sprs.size() * sizeof(uint16_t);	// sprs
	return sz;
}

void PackComplex::Action::
PackToBin(uint8_t** ptr, const std::vector<const PackNode*>& children) const
{
	pack_str(m_name, ptr);

	uint16_t n = m_sprs.size();
	pack(n, ptr);
	for (int i = 0; i < n; ++i) {
		uint16_t idx = 0xffff;
		for (int j = 0, m = children.size(); j < m; ++j) {
			if (children[j] == m_sprs[i]) {
				idx = j;
				break;
			}
		}
		assert(idx != 0xffff);
		pack(idx, ptr);
	}
}

}