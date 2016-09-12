#include "PackScale9Spr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"

#include <easyscale9.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <SM_Vector.h>
#include <simp/NodeScale9Spr.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackScale9Spr::PackScale9Spr(const escale9::Sprite* spr)
{
	Init(spr);
}

void PackScale9Spr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, 
									float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"scale9_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));

	lua::connect(gen, 3, 
		lua::assign("sym_id", m_sym->GetID()), 
		lua::assign("width", m_width),
		lua::assign("height", m_height));

	gen.detab();
	gen.line("},");
}

int PackScale9Spr::SizeOfUnpackFromBin() const
{
	return simp::NodeScale9Spr::Size();
}

int PackScale9Spr::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint32_t);			// sym id
	sz += sizeof(uint16_t);			// width
	sz += sizeof(uint16_t);			// height
	return sz;
}

void PackScale9Spr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, 
							  float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = TYPE_SCALE9_SPR;
	pack(type, ptr);

	uint32_t sym = m_sym->GetID();
	pack(sym, ptr);

	uint16_t w = m_width,
		     h = m_height;
	pack(w, ptr);
	pack(h, ptr);
}

void PackScale9Spr::Init(const escale9::Sprite* spr)
{
	m_sym = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));

	sm::vec2 sz = spr->GetScale9().GetSize();
	m_width = sz.x;
	m_height = sz.y;
}

}