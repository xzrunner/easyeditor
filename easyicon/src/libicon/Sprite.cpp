#include "Sprite.h"
#include "Symbol.h"
#include "SpritePropertySetting.h"

namespace eicon
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::IconSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::IconSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, s2::IconSprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const CU_STR& dir)
{
	ee::Sprite::Load(val);
	SetProcess(val["process"].asDouble());
}

void Sprite::Store(Json::Value& val, const CU_STR& dir) const
{
	ee::Sprite::Store(val);
	val["process"] = m_process;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, std::dynamic_pointer_cast<Sprite>(shared_from_this()));
}

ee::SprPtr Sprite::Create(const std::shared_ptr<ee::Symbol>& sym) 
{
	return std::make_shared<Sprite>(sym);
}

}