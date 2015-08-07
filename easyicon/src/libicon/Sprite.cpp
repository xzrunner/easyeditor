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
	: ISprite(sprite)
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
	d2d::SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(d2d::ISymbol* symbol)
{
	Symbol* icon = dynamic_cast<Symbol*>(symbol);
	if (m_symbol != symbol && icon)
	{
		m_symbol->Release();
		symbol->Retain();

		m_symbol = icon;
	}
}

void Sprite::Load(const Json::Value& val)
{
	ISprite::Load(val);
	SetProcess(val["process"].asDouble());
}

void Sprite::Store(Json::Value& val) const
{
	ISprite::Store(val);
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