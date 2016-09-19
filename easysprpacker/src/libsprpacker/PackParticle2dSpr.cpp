#include "PackParticle2dSpr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "to_int.h"

#include <easyparticle2d.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeParticle2dSpr.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackParticle2dSpr::PackParticle2dSpr(const eparticle2d::Sprite* spr)
{
	Init(spr);
}

void PackParticle2dSpr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"p2d_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));
	if (!m_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + m_name + "\"");
	}

	lua::connect(gen, 1, 
		lua::assign("p2d_id", m_sym->GetID()));

	lua::connect(gen, 2, 
		lua::assign("is_loop", m_loop), 
		lua::assign("is_local", m_local));

	gen.detab();
	gen.line("},");
}

int PackParticle2dSpr::SizeOfUnpackFromBin() const
{
	return simp::NodeParticle2dSpr::Size();
}

int PackParticle2dSpr::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint32_t);			// p2d id
	sz += sizeof(uint8_t) * 2;		// loop, local
	return sz;
}

void PackParticle2dSpr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = simp::TYPE_P2D_SPR;
	pack(type, ptr);

	uint32_t sym = m_sym->GetID();
	pack(sym, ptr);

	uint8_t loop = bool2int(m_loop);
	pack(loop, ptr);

	uint8_t local = bool2int(m_local);
	pack(local, ptr);
}

bool PackParticle2dSpr::Equal(const eparticle2d::Sprite* spr) const
{
	return m_loop == spr->GetLoop()
		&& m_local == spr->GetLocalModeDraw();
}

void PackParticle2dSpr::Init(const eparticle2d::Sprite* spr)
{
	m_sym = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));

	m_loop	= spr->GetLoop();
	m_local	= spr->GetLocalModeDraw();
}

}