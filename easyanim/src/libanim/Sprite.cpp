#include "Sprite.h"
#include "Symbol.h"
#include "PropertySetting.h"

#include <ee/std_functor.h>

namespace eanim
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::AnimSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::AnimSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::AnimSprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	if (val["animation"].isNull()) {
		return;
	}	

	const Json::Value& anim_val = val["animation"];

	m_loop = anim_val["loop"].asBool();
	m_interval = anim_val["interval"].asDouble();

	m_fps = anim_val["fps"].asInt();
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	Json::Value anim_val;

	anim_val["loop"] = m_loop;
	anim_val["interval"] = m_interval;

	anim_val["fps"] = m_fps;

	val["animation"] = anim_val;
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