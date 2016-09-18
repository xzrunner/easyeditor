#include "AnimSprLoader.h"

#include <sprite2/AnimSprite.h>
#include <simp/from_int.h>
#include <simp/NodeAnimationSpr.h>

namespace gum
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
	if (!m_spr || !val.isMember("animation")) {
		return;
	}

	const Json::Value& anim_val = val["animation"];

	m_spr->SetLoop(anim_val["loop"].asBool());
	m_spr->SetInterval(anim_val["interval"].asDouble());

	m_spr->SetFPS(anim_val["fps"].asInt());

	if (anim_val.isMember("active")) {
		m_spr->SetActive(anim_val["active"].asBool());
	}
}

void AnimSprLoader::LoadBin(const simp::NodeAnimationSpr* node)
{
	if (!m_spr) {
		return;
	}

	m_spr->SetLoop(simp::int2bool(node->loop));
	m_spr->SetInterval(simp::int2float1024x(node->interval));

	m_spr->SetFPS(node->fps);
}

}