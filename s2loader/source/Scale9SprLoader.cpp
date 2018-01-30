#include "s2loader/Scale9SprLoader.h"

#include <sprite2/Scale9Sprite.h>
#include <simp/NodeScale9Spr.h>

namespace s2loader
{

Scale9SprLoader::Scale9SprLoader(s2::Scale9Sprite& spr)
	: m_spr(spr)
{
}

void Scale9SprLoader::LoadJson(const Json::Value& val, const CU_STR& dir)
{
	if (!val.isMember("scale9")) {
		return;
	}

	float w = static_cast<float>(val["scale9"]["width"].asDouble());
	float h = static_cast<float>(val["scale9"]["height"].asDouble());
	m_spr.Resize(w, h);
}

void Scale9SprLoader::LoadBin(const simp::NodeScale9Spr* node)
{
	m_spr.Resize(node->width, node->height);
}

}