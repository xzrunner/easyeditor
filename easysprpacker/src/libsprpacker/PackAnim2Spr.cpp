#include "PackAnim2Spr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"

#include <easyanim2.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeAnim2Spr.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackAnim2Spr::PackAnim2Spr(const libanim2::Sprite* spr)
{
	Init(spr);
}

PackAnim2Spr::~PackAnim2Spr()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void PackAnim2Spr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"anim2_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));

	lua::assign_with_end(gen, "static_time", m_static_time);	

	gen.detab();
	gen.line("},");
}

int PackAnim2Spr::SizeOfUnpackFromBin() const
{
	return simp::NodeAnim2Spr::Size();
}

int PackAnim2Spr::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint32_t);			// sym id
	sz += sizeof(uint32_t);			// static time
	return sz;
}

void PackAnim2Spr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_ANIM2_SPR;
	pack(type, ptr);

	uint32_t sym = m_sym->GetID();
	pack(sym, ptr);

	uint32_t static_time = m_static_time;
	pack(static_time, ptr);
}

bool PackAnim2Spr::Equal(const libanim2::Sprite* spr) const
{
	return m_static_time == spr->GetStaticTime();
}

void PackAnim2Spr::Init(const libanim2::Sprite* spr)
{
	m_sym = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));

	m_static_time = spr->GetStaticTime();
}

}