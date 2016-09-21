#include "IconSprLoader.h"

#include <sprite2/IconSprite.h>
#include <simp/NodeIconSpr.h>
#include <simp/from_int.h>

namespace gum
{

IconSprLoader::IconSprLoader(s2::IconSprite* spr)
	: m_spr(spr)
{
	if (m_spr) {
		m_spr->AddReference();
	}
}

IconSprLoader::~IconSprLoader()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

void IconSprLoader::LoadJson(const Json::Value& val, const std::string& dir)
{
}

void IconSprLoader::LoadBin(const simp::NodeIconSpr* node)
{
	m_spr->SetProcess(simp::int2float1024x(node->process));
}

}