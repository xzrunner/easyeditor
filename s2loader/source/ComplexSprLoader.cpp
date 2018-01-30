#include "s2loader/ComplexSprLoader.h"

#include <sprite2/ComplexSprite.h>
#include <simp/NodeComplexSpr.h>

namespace s2loader
{

ComplexSprLoader::ComplexSprLoader(s2::ComplexSprite& spr)
	: m_spr(spr)
{
}

void ComplexSprLoader::LoadJson(const Json::Value& val, const CU_STR& dir)
{
	if (!val.isMember("complex")) {
		return;
	}

	int action = val["complex"]["action"].asInt();
	m_spr.SetAction(action);
}

void ComplexSprLoader::LoadBin(const simp::NodeComplexSpr* node)
{
	int action = node->action;
	if (action == 0xffff) {
		action = -1;
	}
	m_spr.SetAction(action);
}

}