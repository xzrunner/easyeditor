#include "Scale9SprLoader.h"

#include <sprite2/Scale9Sprite.h>

namespace glue
{

Scale9SprLoader::Scale9SprLoader(s2::Scale9Sprite* spr)
	: m_spr(spr)
{
	if (m_spr) {
		m_spr->AddReference();
	}
}

Scale9SprLoader::~Scale9SprLoader()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

void Scale9SprLoader::LoadJson(const Json::Value& val, const std::string& dir)
{
	if (!m_spr || val["scale9"].isNull()) {
		return;
	}

	float w = static_cast<float>(val["scale9"]["width"].asDouble());
	float h = static_cast<float>(val["scale9"]["height"].asDouble());
	m_spr->Resize(w, h);
}

}