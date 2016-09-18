#include "PackParticle3dSpr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "to_int.h"

#include <easyparticle3d.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeParticle3dSpr.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackParticle3dSpr::PackParticle3dSpr(const eparticle3d::Sprite* spr)
{
	Init(spr);
}

void PackParticle3dSpr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"p3d_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));
	if (!m_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + m_name + "\"");
	}

	lua::connect(gen, 1, 
		lua::assign("p3d_id", m_sym->GetID()));

	lua::connect(gen, 4, 
		lua::assign("is_loop", m_loop), 
		lua::assign("is_local", m_local), 
		lua::assign("is_alone", m_alone), 
		lua::assign("is_reuse", m_reuse));

	gen.detab();
	gen.line("},");
}

int PackParticle3dSpr::SizeOfUnpackFromBin() const
{
	return simp::NodeParticle3dSpr::Size();
}

int PackParticle3dSpr::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint32_t);			// p3d id
	sz += sizeof(uint8_t) * 4;		// loop, local, alone, reuse
	return sz;
}

void PackParticle3dSpr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = simp::TYPE_P3D_SPR;
	pack(type, ptr);

	uint32_t sym = m_sym->GetID();
	pack(sym, ptr);

	uint8_t loop = bool2int(m_loop);
	pack(loop, ptr);

	uint8_t local = bool2int(m_local);
	pack(local, ptr);

	uint8_t alone = bool2int(m_alone);
	pack(alone, ptr);

	uint8_t reuse = bool2int(m_reuse);
	pack(reuse, ptr);
}

void PackParticle3dSpr::Init(const eparticle3d::Sprite* spr)
{
	m_sym = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));

	m_loop	= spr->IsLoop();
	m_local	= spr->IsLocalModeDraw();
	m_alone	= spr->IsAlone();
	m_reuse	= spr->IsReuse();
}

}