#include "s2loader/AnimSprLoader.h"

#include <sprite2/AnimSprite.h>
#include <sprite2/UpdateParams.h>
#include <simp/NodeAnimationSpr.h>
#include <bs/FixedPointNum.h>
#include <s2s/AnimSpr.h>

namespace s2loader
{

AnimSprLoader::AnimSprLoader(s2::AnimSprite& spr)
	: m_spr(spr)
{
}

void AnimSprLoader::LoadJson(const Json::Value& val, const CU_STR& dir)
{
	if (!val.isMember("animation")) {
		return;
	}

	const Json::Value& anim_val = val["animation"];

	m_spr.SetLoop(anim_val["loop"].asBool());
	m_spr.SetInterval(static_cast<float>(anim_val["interval"].asDouble()));

	m_spr.SetFPS(anim_val["fps"].asInt());

	m_spr.SetStartRandom(s2::UpdateParams(), anim_val["start_random"].asBool());

	if (anim_val.isMember("active")) {
		m_spr.SetActive(anim_val["active"].asBool(), nullptr);
	}
}

void AnimSprLoader::LoadBin(const simp::NodeAnimationSpr* node)
{
	m_spr.SetLoop(bs::int2bool(node->loop));
	m_spr.SetInterval(bs::int2float(node->interval, 1024));

	m_spr.SetStartRandom(s2::UpdateParams(), bs::int2bool(node->start_random));

	m_spr.SetFPS(node->fps);
}

void AnimSprLoader::LoadSns(const s2s::NodeSpr* spr)
{
	auto anim_spr = dynamic_cast<const s2s::AnimSpr*>(spr);

	m_spr.SetLoop(anim_spr->IsLoop());
	m_spr.SetInterval(anim_spr->GetInterval());

	m_spr.SetFPS(anim_spr->GetFPS());

	m_spr.SetStartRandom(s2::UpdateParams(), anim_spr->IsStartRandom());
}

}