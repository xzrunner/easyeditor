#include "Sprite.h"

namespace emesh
{

Sprite::Sprite()
	: m_symbol(NULL)
{
}

Sprite::Sprite(const Sprite& s)
	: m_symbol(s.m_symbol)
{
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
}

Sprite::~Sprite()
{
//	// todo: not remove from SymbolMgr
// 	if (m_symbol)
// 	{
// 		m_symbol->release();
// 		m_symbol = NULL;
// 	}
}

Sprite* Sprite::clone() const
{
	return new Sprite(*this);
}

const Symbol& Sprite::getSymbol() const
{
	return *m_symbol;
}

void Sprite::setSymbol(d2d::ISymbol* symbol)
{
	Symbol* mesh = dynamic_cast<Symbol*>(symbol);
	if (mesh) m_symbol = mesh;
}

void Sprite::loadBodyFromFile()
{

}

}