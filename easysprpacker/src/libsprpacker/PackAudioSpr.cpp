#include "PackAudioSpr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"

#include <easyaudio.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeAudioSpr.h>
#include <simp/simp_types.h>
#include <bs/FixedPointNum.h>

namespace esprpacker
{

PackAudioSpr::PackAudioSpr(const std::shared_ptr<eaudio::Sprite>& spr)
{
	m_sym = PackNodeFactory::Instance()->Create(
		std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol()));

	m_volume = spr->GetVolume();

	m_offset   = spr->GetAudioOffset();
	m_duration = spr->GetAudioDuration();

	m_fade_in  = spr->GetFadeIn();
	m_fade_out = spr->GetFadeOut();
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

	lua::connect(gen, 5,
		lua::assign("volume", m_volume),
		lua::assign("offset", m_offset),
		lua::assign("duration", m_duration),
		lua::assign("fade_in", m_fade_in),
		lua::assign("fade_out", m_fade_out));

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
	sz += sizeof(float) * 5;        // volume, offset, duration, fade_in, fade_out
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

	float volume = m_volume;
	pack(volume, ptr);

	float offset = m_offset;
	pack(offset, ptr);

	float duration = m_duration;
	pack(duration, ptr);

	float fade_in = m_fade_in;
	pack(fade_in, ptr);

	float fade_out = m_fade_out;
	pack(fade_out, ptr);
}

}