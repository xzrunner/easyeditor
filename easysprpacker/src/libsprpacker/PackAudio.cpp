#include "PackAudio.h"
#include "PackAudioIDMgr.h"
#include "binary_io.h"

#include <easyaudio.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <gum/FilepathHelper.h>
#include <simp/NodeAudio.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackAudio::PackAudio(const eaudio::Symbol* sym)
{
	m_audio_name = gum::FilepathHelper::Filename(sym->GetFilepath());
	m_audio_id = PackAudioIDMgr::Instance()->Query(m_audio_name);
}

void PackAudio::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"audio\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));

	lua::connect(gen, 2, 
		lua::assign("audio_name", m_audio_name),
		lua::assign("audio_id", m_audio_id));

	gen.detab();
	gen.line("},");
}

int PackAudio::SizeOfUnpackFromBin() const
{
	return simp::NodeAudio::Size();
}

int PackAudio::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);						// id
	sz += sizeof(uint8_t);						// type
	sz += sizeof(uint32_t);						// audio id
	return sz;
}

void PackAudio::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_AUDIO;
	pack(type, ptr);

	uint32_t audio_id = m_audio_id;
	pack(audio_id, ptr);
}

}