#include "AnimSprLoader.h"

#include <sprite2/AnimSprite.h>

namespace glue
{

AnimSprLoader::AnimSprLoader(s2::AnimSprite* spr)
	: m_spr(spr)
{
	if (m_spr) {
		m_spr->AddReference();
	}
}

AnimSprLoader::~AnimSprLoader()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

void AnimSprLoader::LoadJson(const Json::Value& val, const std::string& dir)
{
	if (!m_spr || val["animation"].isNull()) {
		return;
	}

	const Json::Value& anim_val = val["animation"];

	m_spr->SetLoop(anim_val["loop"].asBool());
	m_spr->SetInterval(anim_val["interval"].asDouble());

	m_spr->SetFPS(anim_val["fps"].asInt());

	m_spr->SetActive(anim_val["active"].asBool());
}

}