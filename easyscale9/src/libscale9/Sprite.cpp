#include "Sprite.h"
#include "Symbol.h"
#include "SpritePropertySetting.h"

namespace escale9
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::Scale9Sprite(spr)
	, ee::Sprite(spr)
{
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::Scale9Sprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	if (!val["scale9"].isNull()) {
		float w = val["scale9"]["width"].asDouble();
		float h = val["scale9"]["height"].asDouble();
		m_s9.SetSize(w, h);
	}
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	sm::vec2 sz = m_s9.GetSize();
	val["scale9"]["width"] = sz.x;
	val["scale9"]["height"] = sz.y;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, this);
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}