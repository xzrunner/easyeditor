#include "P3dSprLoader.h"

#include <sprite2/Particle3dSprite.h>

namespace gum
{

P3dSprLoader::P3dSprLoader(s2::Particle3dSprite* spr)
	: m_spr(spr)
{
	if (m_spr) {
		m_spr->AddReference();
	}
}

P3dSprLoader::~P3dSprLoader()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

void P3dSprLoader::LoadJson(const Json::Value& val, const std::string& dir)
{
	if (!m_spr || !val.isMember("particle3d")) {
		return;
	}

	const Json::Value& p_val = val["particle3d"];

	bool alone = false;
	if (p_val.isMember("alone")) {
		alone = p_val["alone"].asBool();
	}
	m_spr->SetAlone(alone);

	bool reuse = false;
	if (p_val.isMember("reuse")) {
		reuse = p_val["reuse"].asBool();
	}
	m_spr->SetReuse(reuse);

	bool loop = false;
	if (p_val.isMember("loop")) {
		loop = p_val["loop"].asBool();
	}
	m_spr->SetLoop(loop);

	bool local_mode_draw = false;
	if (p_val.isMember("loop")) {
		local_mode_draw = p_val["local_mode_draw"].asBool();
	}
	m_spr->SetLocalModeDraw(local_mode_draw);
}

}