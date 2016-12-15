#include "PackComplex.h"
#include "PackNodeFactory.h"
#include "binary_io.h"

#include <easycomplex.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <sprite2/S2_Sprite.h>
#include <simp/simp_types.h>
#include <simp/simp_define.h>
#include <simp/NodeComplex.h>

namespace esprpacker
{

PackComplex::PackComplex(const ecomplex::Symbol* sym)
{
	Init(sym);
}

PackComplex::~PackComplex()
{
	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFunctor<PackNode>());
	for (int i = 0, n = m_actions.size(); i < n; ++i) {
		for_each(m_actions[i].m_sprs.begin(), m_actions[i].m_sprs.end(), cu::RemoveRefFunctor<PackNode>());
	}
}

void PackComplex::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"complex\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));
	if (!GetName().empty()) {
		lua::assign_with_end(gen, "export", "\"" + GetName() + "\"");
	}

	// scissor
	lua::connect(gen, 4, 
		lua::assign("xmin", m_scissor.xmin), 
		lua::assign("ymin", m_scissor.ymin),
		lua::assign("xmax", m_scissor.xmax),
		lua::assign("ymax", m_scissor.ymax));

	// children
	{
		lua::TableAssign ta(gen, "children", true);
		for (int i = 0, n = m_children.size(); i < n; ++i) {
			lua::TableAssign assign(gen, "", true);
			std::string sid = lua::assign("id", ee::StringHelper::ToString(m_children[i]->GetID()));
			lua::connect(gen, 1, sid);
			m_children_trans[i].PackToLua(gen);
		}
	}

	// actions
	{
		lua::TableAssign ta(gen, "actions", true);
		for (int i = 0, n = m_actions.size(); i < n; ++i) {
			const Action& action = m_actions[i];
			
			lua::TableAssign ta(gen, "", true);

			gen.line(lua::assign("action", "\"" + action.m_name + "\"") + ",");
			for (int j = 0, m = action.m_sprs.size(); j < m; ++j) {
				std::string str = lua::assign("idx", ee::StringHelper::ToString(QueryIndex(action.m_sprs[j])));
				lua::tableassign(gen, "", 1, str);
			}
		}
	}

	gen.detab();
	gen.line("},");
}

int PackComplex::SizeOfUnpackFromBin() const
{
	int sz = simp::NodeComplex::Size();
	sz += simp::SIZEOF_POINTER * m_children.size();
	for (int i = 0, n = m_actions.size(); i < n; ++i) {
		sz += m_actions[i].SizeOfUnpackFromBin();
	}
	sz += sizeof(uint32_t) * m_children_trans.size();
	for (int i = 0, n = m_children_trans.size(); i < n; ++i) {
		sz += m_children_trans[i].SizeOfUnpackFromBin();
	}
	return sz;
}

int PackComplex::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint16_t) * 4;		// scissor
	// actions
	sz += sizeof(uint16_t);			
	for (int i = 0, n = m_actions.size(); i < n; ++i) {
		sz += m_actions[i].SizeOfPackToBin();
	}
	// children
	sz += sizeof(uint16_t);
	sz += sizeof(uint32_t) * m_children.size();
	for (int i = 0, n = m_children_trans.size(); i < n; ++i) {
		sz += m_children_trans[i].SizeOfPackToBin();
	}
	return sz;
}

void PackComplex::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_COMPLEX;
	pack(type, ptr);

	// scissor
	int16_t xmin = m_scissor.xmin,
		    ymin = m_scissor.ymin,
			xmax = m_scissor.xmax,
			ymax = m_scissor.ymax;
	pack(xmin, ptr);
	pack(ymin, ptr);
	pack(xmax, ptr);
	pack(ymax, ptr);

	// actions
	uint16_t action_n = m_actions.size();
	pack(action_n, ptr);
	for (int i = 0; i < action_n; ++i) {
		m_actions[i].PackToBin(ptr, *this);
	}

	// children
	uint16_t children_n = m_children.size();
	pack(children_n, ptr);
	for (int i = 0; i < children_n; ++i) {
		uint32_t id = m_children[i]->GetID();
		pack(id, ptr);
		m_children_trans[i].PackToBin(ptr);
	}
}

void PackComplex::Init(const ecomplex::Symbol* sym)
{
	const std::vector<s2::Sprite*>& children = sym->GetChildren();
	m_children.reserve(children.size());
	m_children_trans.reserve(children.size());
	for (int i = 0, n = children.size(); i < n; ++i) {
		m_children.push_back(PackNodeFactory::Instance()->Create(
			dynamic_cast<ee::Sprite*>(children[i])));
		m_children_trans.push_back(*children[i]);
	}

	const std::vector<s2::ComplexSymbol::Action>& actions = sym->GetActions();
	for (int i = 0, n = actions.size(); i < n; ++i) 
	{
		const s2::ComplexSymbol::Action& src = actions[i];
		Action dst;
		dst.m_name = src.name;
		dst.m_sprs.reserve(src.sprs.size());
		for (int j = 0, m = src.sprs.size(); j < m; ++j) 
		{
			int idx = -1;
			for (int k = 0; k < children.size(); ++k) {
				if (src.sprs[j] == children[k]) {
					idx = k;
				}
			}
			assert(idx != -1);

			m_children[idx]->AddReference();
			dst.m_sprs.push_back(m_children[idx]);
		}
		m_actions.push_back(dst);
	}

	m_scissor = sym->GetScissor();
}

int PackComplex::QueryIndex(const PackNode* node) const
{
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (m_children[i] == node) {
			return i;
		}
	}
	assert(0);
	return -1;
}

/************************************************************************/
/* class PackComplex::Action                                                                     */
/************************************************************************/

int PackComplex::Action::
SizeOfUnpackFromBin() const 
{
	int sz = simp::NodeComplex::ActionSize();
	sz += sizeof_unpack_str(m_name);
	sz += sizeof(uint16_t) * m_sprs.size();
	return sz;
}

int PackComplex::Action::
SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof_pack_str(m_name); // name
	// sprs
	sz += sizeof(uint16_t);	
	sz += m_sprs.size() * sizeof(uint16_t);
	return sz;
}

void PackComplex::Action::
PackToBin(uint8_t** ptr, const PackComplex& complex) const
{
	pack_str(m_name, ptr);

	uint16_t n = m_sprs.size();
	pack(n, ptr);
	for (int i = 0; i < n; ++i) {
		int idx = complex.QueryIndex(m_sprs[i]);
		assert(idx >= 0 && idx < 0xffff);
		uint16_t _idx = idx;
		pack(_idx, ptr);
	}
}

}