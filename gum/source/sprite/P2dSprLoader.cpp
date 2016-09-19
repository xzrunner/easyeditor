#include "P2dSprLoader.h"

#include <sprite2/Particle2dSprite.h>
#include <simp/from_int.h>
#include <simp/NodeParticle2dSpr.h>

namespace gum
{

P2dSprLoader::P2dSprLoader(s2::Particle2dSprite* spr)
	: m_spr(spr)
{
	if (m_spr) {
		m_spr->AddReference();
	}
}

P2dSprLoader::~P2dSprLoader()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

void P2dSprLoader::LoadJson(const Json::Value& val, const std::string& dir)
{
	if (!m_spr || !val.isMember("particle2d")) {
		return;
	}

	const Json::Value& p_val = val["particle2d"];

	m_spr->SetLoop(p_val["loop"].asBool());
	m_spr->SetLocalModeDraw(p_val["local_mode_draw"].asBool());
}

void P2dSprLoader::LoadBin(const simp::NodeParticle2dSpr* node)
{
	if (!m_spr) {
		return;
	}

	m_spr->SetLoop(simp::int2bool(node->loop));
	m_spr->SetLocalModeDraw(simp::int2bool(node->local));
}

}