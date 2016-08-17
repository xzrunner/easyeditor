#include "Sprite.h"
#include "Symbol.h"
#include "SpritePropertySetting.h"

namespace eicon
{

Sprite::Sprite()
	: m_process(1)
{
}

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::IconSprite(spr)
	, ee::Sprite(spr)
	, m_process(spr.m_process)
{
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::IconSprite(sym)
	, ee::Sprite(sym)
	, m_process(1)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);
	SetProcess(val["process"].asDouble());
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);
	val["process"] = m_process;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, this);
}

void Sprite::SetProcess(float process)
{
	m_process = process;
}

ee::Sprite* Sprite::Create(ee::Symbol* symbol) 
{
	return new Sprite(static_cast<Symbol*>(symbol));
}

}