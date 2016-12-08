#include "PackShapeSpr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "to_int.h"

#include <easyshape.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeShapeSpr.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackShapeSpr::PackShapeSpr(const eshape::Sprite* spr)
{
	m_sym = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));
}

void PackShapeSpr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"shape_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));

	lua::connect(gen, 1, 
		lua::assign("shape_id", m_sym->GetID()));

	gen.detab();
	gen.line("},");
}

int PackShapeSpr::SizeOfUnpackFromBin() const
{
	return simp::NodeShapeSpr::Size();
}

int PackShapeSpr::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint32_t);			// shape id
	return sz;
}

void PackShapeSpr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_SHAPE_SPR;
	pack(type, ptr);

	uint32_t sym = m_sym->GetID();
	pack(sym, ptr);
}

}