#include "ComplexSprLoader.h"

#include <sprite2/ComplexSprite.h>

namespace glue
{

ComplexSprLoader::ComplexSprLoader(s2::ComplexSprite* spr)
	: m_spr(spr)
{
	if (m_spr) {
		m_spr->AddReference();
	}
}

ComplexSprLoader::~ComplexSprLoader()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

void ComplexSprLoader::LoadJson(const Json::Value& val, const std::string& dir)
{
	if (!m_spr || val["complex"].isNull()) {
		return;
	}

	int action = val["complex"]["action"].asInt();
	m_spr->SetAction(action);
}

}