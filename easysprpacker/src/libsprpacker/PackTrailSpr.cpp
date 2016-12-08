#include "PackTrailSpr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "to_int.h"

#include <easytrail.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeTrailSpr.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackTrailSpr::PackTrailSpr(const etrail::Sprite* spr)
{
	m_sym = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));
}

void PackTrailSpr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"trail_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));

	lua::connect(gen, 1, 
		lua::assign("trail_id", m_sym->GetID()));

	gen.detab();
	gen.line("},");
}

int PackTrailSpr::SizeOfUnpackFromBin() const
{
	return simp::NodeTrailSpr::Size();
}

int PackTrailSpr::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint32_t);			// trail id
	return sz;
}

void PackTrailSpr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_TRAIL_SPR;
	pack(type, ptr);

	uint32_t sym = m_sym->GetID();
	pack(sym, ptr);
}

}