#include "PackComplexSpr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"

#include <easycomplex.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeComplexSpr.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackComplexSpr::PackComplexSpr(const ecomplex::Sprite* spr)
{
	Init(spr);
}

void PackComplexSpr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"complex_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));

	lua::connect(gen, 2, 
		lua::assign("sym_id", m_sym->GetID()), 
		lua::assign("action", m_action));

	gen.detab();
	gen.line("},");
}

int PackComplexSpr::SizeOfUnpackFromBin() const
{
	return simp::NodeComplexSpr::Size();
}

int PackComplexSpr::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint32_t);			// sym id
	sz += sizeof(uint16_t);			// action
	return sz;
}

void PackComplexSpr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = TYPE_COMPLEX_SPR;
	pack(type, ptr);

	uint32_t sym = m_sym->GetID();
	pack(sym, ptr);

	uint16_t action = m_action;
	pack(action, ptr);
}

void PackComplexSpr::Init(const ecomplex::Sprite* spr)
{
	m_sym = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));

	m_action = spr->GetAction();
}

}