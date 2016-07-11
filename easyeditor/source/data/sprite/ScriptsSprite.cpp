#include "ScriptsSprite.h"
#include "SpriteFactory.h"
#include "scripts.h"

#include <sprite2/DummySprite.h>

namespace ee
{

ScriptsSprite::ScriptsSprite()
	: m_symbol(NULL)
{
	m_core = new s2::DummySprite(this);
}

ScriptsSprite::ScriptsSprite(ScriptsSymbol* symbol)
	: m_symbol(symbol)
{
	m_core = new s2::DummySprite(this);

	m_symbol->Retain();
	scripts_do_string(symbol->GetContent().c_str());
}

ScriptsSprite::~ScriptsSprite()
{
	m_core->RemoveReference();

	if (m_symbol) {
		m_symbol->Release();
	}
}

ScriptsSprite* ScriptsSprite::Clone() const
{
	ScriptsSprite* sprite = new ScriptsSprite(*this);
	SpriteFactory::Instance()->Insert(sprite);
	return sprite;
}

const ScriptsSymbol& ScriptsSprite::GetSymbol() const
{
	return *m_symbol;
}

void ScriptsSprite::SetSymbol(Symbol* symbol)
{
	Sprite::SetSymbol(&m_symbol, symbol);
}

}