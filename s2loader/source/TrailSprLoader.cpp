#include "s2loader/TrailSprLoader.h"

#include <sprite2/TrailSprite.h>
#include <simp/NodeTrailSpr.h>

namespace s2loader
{

TrailSprLoader::TrailSprLoader(const std::shared_ptr<s2::TrailSprite>& spr)
	: m_spr(spr)
{
}

void TrailSprLoader::LoadJson(const Json::Value& val, const std::string& dir)
{
	if (!m_spr || !val.isMember("trail")) {
		return;
	}

	const Json::Value& t_val = val["trail"];

	bool local = false;
	if (t_val.isMember("local")) {
		local = t_val["local"].asBool();
	}
	m_spr->SetLocal(local);
}

void TrailSprLoader::LoadBin(const simp::NodeTrailSpr* node)
{
	if (!m_spr) {
		return;
	}

	// todo
}

}