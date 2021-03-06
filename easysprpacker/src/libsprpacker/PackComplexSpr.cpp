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

PackComplexSpr::PackComplexSpr(const std::shared_ptr<ecomplex::Sprite>& spr)
{
	Init(spr);
}

PackComplexSpr::~PackComplexSpr()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void PackComplexSpr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"complex_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));

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

void PackComplexSpr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_COMPLEX_SPR;
	pack(type, ptr);

	uint32_t sym = m_sym->GetID();
	pack(sym, ptr);

	uint16_t action = m_action;
	pack(action, ptr);
}

bool PackComplexSpr::Equal(const std::shared_ptr<ecomplex::Sprite>& spr) const
{
	return m_action == spr->GetAction();
}

void PackComplexSpr::Init(const std::shared_ptr<ecomplex::Sprite>& spr)
{
	m_sym = PackNodeFactory::Instance()->Create(
		std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol()));

	m_action = spr->GetAction();
}

}