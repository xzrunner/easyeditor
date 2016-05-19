#include "Sprite.h"
#include "SpritePropertySetting.h"

#include <ee/SpriteFactory.h>

#include <sprite2/DummySprite.h>

namespace eicon
{

Sprite::Sprite()
	: m_symbol(NULL)
	, m_process(1)
{
	m_core = new s2::DummySprite(this);
}

Sprite::Sprite(const Sprite& sprite)
	: ee::Sprite(sprite)
	, m_symbol(sprite.m_symbol)
	, m_process(sprite.m_process)
{
	m_core = new s2::DummySprite(*static_cast<s2::DummySprite*>(sprite.m_core));

	m_symbol->Retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
	, m_process(1)
{
	m_core = new s2::DummySprite(this);

	m_symbol->Retain();
	BuildBounding();	
}

Sprite::~Sprite()
{
	delete m_core; m_core = NULL;

	if (m_symbol) {
		m_symbol->Release();
	}
}

Sprite* Sprite::Clone() const
{
	Sprite* sprite = new Sprite(*this);
	ee::SpriteFactory::Instance()->Insert(sprite);
	return sprite;
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(ee::Symbol* symbol)
{
	ee::Sprite::SetSymbol(&m_symbol, symbol);
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
	BuildBounding();
}

}