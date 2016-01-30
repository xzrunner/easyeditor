#include "Sprite.h"
#include "SpritePropertySetting.h"

namespace eicon
{

Sprite::Sprite()
	: m_symbol(NULL)
	, m_process(1)
{
}

Sprite::Sprite(const Sprite& sprite)
	: Sprite(sprite)
	, m_symbol(sprite.m_symbol)
	, m_process(sprite.m_process)
{
	m_symbol->Retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
	, m_process(1)
{
	m_symbol->Retain();
	BuildBounding();	
}

Sprite::~Sprite()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

Sprite* Sprite::Clone() const
{
	Sprite* sprite = new Sprite(*this);
	d2d::SpriteFactory::Instance()->Insert(sprite);
	return sprite;
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(d2d::Symbol* symbol)
{
	d2d::Sprite::SetSymbol(&m_symbol, symbol);
}

void Sprite::Load(const Json::Value& val)
{
	Sprite::Load(val);
	SetProcess(val["process"].asDouble());
}

void Sprite::Store(Json::Value& val) const
{
	Sprite::Store(val);
	val["process"] = m_process;
}

d2d::IPropertySetting* Sprite::CreatePropertySetting(d2d::EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, this);
}

void Sprite::SetProcess(float process)
{
	m_process = process;
	BuildBounding();
}

}