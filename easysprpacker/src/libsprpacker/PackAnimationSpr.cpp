#include "PackAnimationSpr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "to_int.h"

#include <easyanim.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeAnimationSpr.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackAnimationSpr::PackAnimationSpr(const libanim::Sprite* spr)
{
	Init(spr);
}

PackAnimationSpr::~PackAnimationSpr()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void PackAnimationSpr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"anim_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));

	lua::connect(gen, 4, 
		lua::assign("sym_id", m_sym->GetID()), 
		lua::assign("loop", m_loop),
		lua::assign("interval", m_interval),
		lua::assign("fps", m_fps));

	gen.detab();
	gen.line("},");
}

int PackAnimationSpr::SizeOfUnpackFromBin() const
{
	return simp::NodeAnimationSpr::Size();
}

int PackAnimationSpr::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint32_t);			// sym id
	sz += sizeof(uint8_t);			// loop
	sz += sizeof(uint32_t);			// interval
	sz += sizeof(uint16_t);			// fps
	return sz;
}

void PackAnimationSpr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_ANIM_SPR;
	pack(type, ptr);

	uint32_t sym = m_sym->GetID();
	pack(sym, ptr);

	uint8_t loop = bool2int(m_loop);
	pack(loop, ptr);

	uint32_t interval = float1024x2int(m_interval);
	pack(interval, ptr);

	uint16_t fps = m_fps;
	pack(fps, ptr);
}

bool PackAnimationSpr::Equal(const libanim::Sprite* spr) const
{
	return m_loop == spr->IsLoop()
		&& m_interval == spr->GetInterval()
		&& m_fps == spr->GetFPS();
}

void PackAnimationSpr::Init(const libanim::Sprite* spr)
{
	m_sym = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));

	m_loop = spr->IsLoop();
	m_interval = spr->GetInterval();

	m_fps = spr->GetFPS();
}

}