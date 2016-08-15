#include "Sprite.h"
#include "Symbol.h"
#include "SpritePropertySetting.h"

namespace eicon
{

Sprite::Sprite()
	: m_process(1)
{
}

Sprite::Sprite(const Sprite& sprite)
	: ee::Sprite(sprite)
	, m_process(sprite.m_process)
{
}

Sprite::Sprite(Symbol* symbol)
	: ee::Sprite(symbol)
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