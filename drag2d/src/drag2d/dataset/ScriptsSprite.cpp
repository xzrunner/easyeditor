#include "ScriptsSprite.h"
#include "SpriteFactory.h"

#include "common/scripts.h"

namespace d2d
{

ScriptsSprite::ScriptsSprite()
	: m_symbol(NULL)
{
}

ScriptsSprite::ScriptsSprite(ScriptsSymbol* symbol)
	: m_symbol(symbol)
{
	m_symbol->Retain();
	scripts_do_string(symbol->getContent().c_str());
}

ScriptsSprite::~ScriptsSprite()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

ScriptsSprite* ScriptsSprite::Clone() const
{
	ScriptsSprite* sprite = new ScriptsSprite(*this);
	SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const ScriptsSymbol& ScriptsSprite::GetSymbol() const
{
	return *m_symbol;
}

void ScriptsSprite::SetSymbol(ISymbol* symbol)
{
	ISprite::SetSymbol(&m_symbol, symbol);
}

}