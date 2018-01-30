#include "s2loader/AudioSprLoader.h"

#include <sprite2/AudioSprite.h>
#include <uniaudio/AudioContext.h>
#include <uniaudio/Source.h>
#include <simp/NodeAudioSpr.h>
#include <gum/Audio.h>

namespace s2loader
{

AudioSprLoader::AudioSprLoader(s2::AudioSprite& spr)
	: m_spr(spr)
{
}

void AudioSprLoader::LoadBin(const CU_STR& filepath, const simp::NodeAudioSpr* node)
{
	auto ctx = gum::Audio::Instance()->GetContext();
	if (!ctx) {
		return;
	}
	auto source = ctx->CreateSource(filepath, true);
	if (!source) {
		return;
	}

	m_spr.SetSource(source);

	m_spr.SetVolume(node->volume);

	m_spr.SetAudioOffset(node->offset);
	m_spr.SetAudioDuration(node->duration);

	m_spr.SetFadeIn(node->fade_in);
	m_spr.SetFadeOut(node->fade_out);
}

}