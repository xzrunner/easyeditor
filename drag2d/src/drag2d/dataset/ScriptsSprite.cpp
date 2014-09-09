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
	m_symbol->retain();
	scripts_do_string(symbol->getContent().c_str());
}
Z
ScriptsSprite::~ScriptsSprite()
{
	if (m_symbol) {
		m_symbol->release();
	}
}

ScriptsSprite* ScriptsSprite::clone() const
{
	ScriptsSprite* sprite = new ScriptsSprite(*this);
	SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const ScriptsSymbol& ScriptsSprite::getSymbol() const
{
	return *m_symbol;
}

void ScriptsSprite::setSymbol(ISymbol* symbol)
{
	ScriptsSymbol* scripts = dynamic_cast<ScriptsSymbol*>(symbol);
	if (m_symbol != symbol && scripts)
	{
		m_symbol->release();
		symbol->retain();

		m_symbol = scripts;
	}
}

void ScriptsSprite::loadBodyFromFile()
{
}

}