#include "s2loader/Anim2SprLoader.h"

#include <sprite2/Anim2Sprite.h>
#include <sprite2/UpdateParams.h>
#include <simp/NodeAnim2Spr.h>

namespace s2loader
{

Anim2SprLoader::Anim2SprLoader(s2::Anim2Sprite& spr)
	: m_spr(spr)
{
}

void Anim2SprLoader::LoadJson(const Json::Value& val, const CU_STR& dir)
{
	if (!val.isMember("anim2")) {
		return;
	}

	const Json::Value& anim_val = val["anim2"];
	m_spr.SetStaticTime(s2::UpdateParams(), anim_val["static_time"].asInt());
}

void Anim2SprLoader::LoadBin(const simp::NodeAnim2Spr* node)
{
	m_spr.SetStaticTime(s2::UpdateParams(), node->static_time);
}

}