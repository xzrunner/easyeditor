#include "Sprite.h"
#include "Symbol.h"
#include "SprPropSetting.h"

#include <gum/TrailSprLoader.h>

namespace etrail
{

Sprite::Sprite()
{
}

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::TrailSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::TrailSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::TrailSprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	const Json::Value& t_val = val["trail"];
	m_local = t_val["local"].asBool();

	gum::TrailSprLoader loader(this);
	loader.LoadJson(val, dir);
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	Json::Value t_val;

	t_val["local"] = m_local;

	val["trail"] = t_val;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new SprPropSetting(stage, this);
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}