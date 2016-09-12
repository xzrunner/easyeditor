#include "PackIcon.h"
#include "binary_io.h"

#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeIcon.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackIcon::PackIcon(const eicon::Symbol* sym)
{
	Init(sym);
}

void PackIcon::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"icon\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));

	lua::assign_with_end(gen, "icon_type", m_type);

	gen.detab();
	gen.line("},");
}

int PackIcon::SizeOfUnpackFromBin() const
{
	return simp::NodeIcon::Size();
}

int PackIcon::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint8_t);			// icon type
	return sz;
}

void PackIcon::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = TYPE_ICON;
	pack(type, ptr);

	uint8_t icon_type = m_type;
	pack(icon_type, ptr);
}

void PackIcon::Init(const eicon::Symbol* sym)
{
	m_type = 0;
}

}