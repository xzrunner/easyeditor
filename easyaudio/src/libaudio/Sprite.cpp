#include "Sprite.h"
#include "Symbol.h"
#include "PropertySetting.h"

#include <ee/SymbolFile.h>
#include <ee/std_functor.h>

#include <easycomplex.h>

#include <uniaudio/AudioData.h>
#include <uniaudio/AudioContext.h>
#include <uniaudio/DecoderFactory.h>
#include <uniaudio/Decoder.h>
#include <uniaudio/openal/Source.h>
#include <sprite2/SymType.h>
#include <gum/Audio.h>

namespace eaudio
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::AudioSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::AudioSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, s2::AudioSprite(sym)
	, ee::Sprite(sym)
{
	LoadResourcesStream(std::dynamic_pointer_cast<ee::Symbol>(sym)->GetFilepath());
}

bool Sprite::Update(const s2::UpdateParams& up) 
{ 
	return false;
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	const Json::Value& audio_val = val["audio"];

	float volume = 1, offset = 0, duration = 0, fade_in = 0, fade_out = 0;
	if (audio_val.isMember("volume")) {
		volume = audio_val["volume"].asFloat();
	}
	if (audio_val.isMember("offset")) {
		offset = audio_val["offset"].asFloat();
	}
	if (audio_val.isMember("duration")) {
		duration = audio_val["duration"].asFloat();
	}
	if (audio_val.isMember("fade_in")) {
		fade_in = audio_val["fade_in"].asFloat();
	}
	if (audio_val.isMember("fade_out")) {
		fade_out = audio_val["fade_out"].asFloat();
	}

	SetVolume(volume);

	SetAudioOffset(offset);
	SetAudioDuration(duration);

	SetFadeIn(fade_in);
	SetFadeOut(fade_out);
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	Json::Value audio_val;

	audio_val["volume"] = GetVolume();

	audio_val["offset"] = GetAudioOffset();
	audio_val["duration"] = GetAudioDuration();

	audio_val["fade_in"]  = GetFadeIn();
	audio_val["fade_out"] = GetFadeOut();

	val["audio"] = audio_val;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new PropertySetting(stage, std::dynamic_pointer_cast<Sprite>(shared_from_this()));
}

ee::SprPtr Sprite::Create(const std::shared_ptr<ee::Symbol>& sym) 
{
	return std::make_shared<Sprite>(sym);
}

bool Sprite::LoadResourcesStatic(const std::string& filepath)
{
	if (ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_COMPLEX)
	{
		std::vector<std::string> children;
		ecomplex::FileLoader::LoadChildren(filepath, children);

		CU_VEC<ua::AudioData*> list;
		for (int i = 0, n = children.size(); i < n; ++i) {
			if (ee::SymbolFile::Instance()->Type(children[i]) == s2::SYM_AUDIO) {
				list.push_back(new ua::AudioData(children[i].c_str()));
			}
		}

		if (list.size() == 0) {
			return false;
		}
		else if (list.size() == 1) {
			ua::AudioContext* ctx = gum::Audio::Instance()->GetContext();
			SetSource(ctx->CreateSource(list[0]));
		}
		else {
			ua::AudioData* data = new ua::AudioData(list);
			ua::AudioContext* ctx = gum::Audio::Instance()->GetContext();
			SetSource(ctx->CreateSource(data));
			delete data;
			for_each(list.begin(), list.end(), ee::DeletePointerFunctor<ua::AudioData>());
		}
	}
	else
	{
		ua::AudioData* data = new ua::AudioData(filepath.c_str());
		ua::AudioContext* ctx = gum::Audio::Instance()->GetContext();
		SetSource(ctx->CreateSource(data));
		delete data;
	}

	return true;
}

bool Sprite::LoadResourcesStream(const std::string& filepath)
{
	auto decoder = ua::DecoderFactory::Create(filepath.c_str());
	if (!decoder) {
		return false;
	}

	ua::AudioContext* ctx = gum::Audio::Instance()->GetContext();
	SetSource(ctx->CreateSource(decoder));

	return true;
}

}