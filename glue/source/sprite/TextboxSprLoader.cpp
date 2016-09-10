#include "TextboxSprLoader.h"
#include "TextboxLoader.h"

#include <sprite2/TextboxSprite.h>

namespace glue
{

TextboxSprLoader::TextboxSprLoader(s2::TextboxSprite* spr)
	: m_spr(spr)
{
	if (m_spr) {
		m_spr->AddReference();
	}
}

TextboxSprLoader::~TextboxSprLoader()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

void TextboxSprLoader::LoadJson(const Json::Value& val, const std::string& dir)
{
	if (!m_spr || !val.isMember("text")) {
		return;
	}

	TextboxLoader loader(m_spr->GetTextbox());
	loader.LoadJson(val["text"]);
}

}