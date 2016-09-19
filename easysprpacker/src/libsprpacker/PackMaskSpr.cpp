#include "PackMaskSpr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "to_int.h"

#include <easymask.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeMaskSpr.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackMaskSpr::PackMaskSpr(const emask::Sprite* spr)
{
	Init(spr);
}

void PackMaskSpr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"mask_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));

	lua::connect(gen, 1, 
		lua::assign("mask_id", m_sym->GetID()));

	gen.detab();
	gen.line("},");
}

int PackMaskSpr::SizeOfUnpackFromBin() const
{
	return simp::NodeMaskSpr::Size();
}

int PackMaskSpr::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint32_t);			// mask id
	return sz;
}

void PackMaskSpr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = simp::TYPE_MASK_SPR;
	pack(type, ptr);

	uint32_t sym = m_sym->GetID();
	pack(sym, ptr);
}

void PackMaskSpr::Init(const emask::Sprite* spr)
{
	m_sym = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));
}

}