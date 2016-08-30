#include "PackIconSpr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "to_int.h"

#include <easyicon.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp_icon.h>
#include <simp_pack.h>

namespace esprpacker
{

PackIconSpr::PackIconSpr(const eicon::Sprite* spr)
{
	Init(spr);
}

void PackIconSpr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"icon_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));

	lua::connect(gen, 2, 
		lua::assign("sym_id", m_sym->GetID()), 
		lua::assign("process", m_process));

	gen.detab();
	gen.line("},");
}

int PackIconSpr::SizeOfUnpackFromBin() const
{
	return SIZEOF_ICON_SPR;
}

int PackIconSpr::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint16_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint32_t);			// sym id
	sz += sizeof(uint16_t);			// process
	return sz;
}

void PackIconSpr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = TYPE_ICON_SPR;
	pack(type, ptr);

	uint32_t sym = m_sym->GetID();
	pack(sym, ptr);

	uint16_t process = float1024x2int(m_process);
	pack(process, ptr);
}

void PackIconSpr::Init(const eicon::Sprite* spr)
{
	m_sym = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));

	m_process = spr->GetProcess();
}

}