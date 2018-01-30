#include "s2loader/P2dSprLoader.h"

#include <sprite2/Particle2dSprite.h>
#include <simp/NodeParticle2dSpr.h>
#include <bs/FixedPointNum.h>

namespace s2loader
{

P2dSprLoader::P2dSprLoader(s2::Particle2dSprite& spr)
	: m_spr(spr)
{
}

void P2dSprLoader::LoadJson(const Json::Value& val, const CU_STR& dir)
{
	if (!val.isMember("particle2d")) {
		return;
	}

	const Json::Value& p_val = val["particle2d"];

	m_spr.SetLoop(p_val["loop"].asBool());
	m_spr.SetLocal(p_val["local_mode_draw"].asBool());
}

void P2dSprLoader::LoadBin(const simp::NodeParticle2dSpr* node)
{
	m_spr.SetLoop(bs::int2bool(node->loop));
	m_spr.SetLocal(bs::int2bool(node->local));
}

}