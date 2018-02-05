#include "s2loader/IconSprLoader.h"

#include <sprite2/IconSprite.h>
#include <simp/NodeIconSpr.h>
#include <bs/FixedPointNum.h>
#include <s2s/IconSpr.h>

namespace s2loader
{

IconSprLoader::IconSprLoader(s2::IconSprite& spr)
	: m_spr(spr)
{
}

void IconSprLoader::LoadJson(const Json::Value& val, const CU_STR& dir)
{
}

void IconSprLoader::LoadBin(const simp::NodeIconSpr* node)
{
	m_spr.SetProcess(bs::int2float(node->process, 1024));
}

void IconSprLoader::LoadSns(const s2s::IconSpr* spr)
{
	m_spr.SetProcess(spr->GetProcess());
}

}