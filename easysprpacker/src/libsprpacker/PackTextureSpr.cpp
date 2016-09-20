#include "PackTextureSpr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "to_int.h"

#include <easytexture.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeTextureSpr.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackTextureSpr::PackTextureSpr(const etexture::Sprite* spr)
{
	m_sym = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));
}

void PackTextureSpr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"texture_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));

	lua::connect(gen, 1, 
		lua::assign("texture_id", m_sym->GetID()));

	gen.detab();
	gen.line("},");
}

int PackTextureSpr::SizeOfUnpackFromBin() const
{
	return simp::NodeTextureSpr::Size();
}

int PackTextureSpr::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint32_t);			// texture id
	return sz;
}

void PackTextureSpr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = simp::TYPE_TEXTURE_SPR;
	pack(type, ptr);

	uint32_t sym = m_sym->GetID();
	pack(sym, ptr);
}

}