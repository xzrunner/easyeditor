#include "Sprite.h"
#include "Symbol.h"
#include "PropertySetting.h"

#include <s2loader/Anim2SprLoader.h>

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

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, s2::Anim2Sprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	s2loader::Anim2SprLoader loader(*this);
	loader.LoadJson(val, dir.c_str());
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
	return new PropertySetting(stage, std::dynamic_pointer_cast<Sprite>(shared_from_this()));
}

ee::SprPtr Sprite::Create(const std::shared_ptr<ee::Symbol>& sym) 
{
	return std::make_shared<Sprite>(sym);
//	return std::make_shared<Sprite>(sym);
}

}