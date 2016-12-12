#include "Sprite.h"
#include "Symbol.h"
#include "PropertySetting.h"

#include <gum/Anim2SprLoader.h>

namespace libanim2
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::Anim2Sprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::Anim2Sprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::Anim2Sprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	gum::Anim2SprLoader loader(this);
	loader.LoadJson(val, dir);
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	Json::Value anim_val;

	anim_val["static_time"] = m_static_time;

	val["anim2"] = anim_val;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new PropertySetting(stage, this);
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}