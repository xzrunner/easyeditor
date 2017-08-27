#include "PackAudioSpr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "to_int.h"

#include <easyaudio.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeAudioSpr.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackAudioSpr::PackAudioSpr(const eaudio::Sprite* spr)
{
	m_sym = PackNodeFactory::Instance()->Create(
		dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));
}

PackAudioSpr::~PackAudioSpr()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void PackAudioSpr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"audio_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));

	lua::connect(gen, 1, 
		lua::assign("audio_id", m_sym->GetID()));

	gen.detab();
	gen.line("},");
}

int PackAudioSpr::SizeOfUnpackFromBin() const
{
	return simp::NodeAudioSpr::Size();
}

int PackAudioSpr::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint32_t);			// audio id
	return sz;
}

void PackAudioSpr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_AUDIO_SPR;
	pack(type, ptr);

	uint32_t sym = m_sym->GetID();
	pack(sym, ptr);
}

}