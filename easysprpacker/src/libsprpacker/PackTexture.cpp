#include "PackTexture.h"
#include "binary_io.h"

#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp_texture.h>

namespace esprpacker
{

PackTexture::PackTexture(const etexture::Symbol* sym)
{
	Init(sym);
}

void PackTexture::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"texture\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));

	lua::assign_with_end(gen, "texture_type", m_type);

	gen.detab();
	gen.line("},");
}

int PackTexture::SizeOfUnpackFromBin() const
{
	return SIZEOF_TEXTURE;
}

int PackTexture::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint16_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint8_t);			// texture_type type
	return sz;
}

void PackTexture::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = TYPE_TEXTURE;
	pack(type, ptr);

	uint8_t texture_type = m_type;
	pack(texture_type, ptr);
}

void PackTexture::Init(const etexture::Symbol* sym)
{
	m_type = 0;
}

}