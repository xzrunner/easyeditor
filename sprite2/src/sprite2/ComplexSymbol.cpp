#include "ComplexSymbol.h"

namespace s2
{

ComplexSymbol::ComplexSymbol(void* ud)
	: Symbol(ud)
{
}

void ComplexSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	
}

void ComplexSymbol::Add(Sprite* spr)
{
	m_children.push_back(spr);
}

void ComplexSymbol::Clear()
{
	// todo release children
	m_children.clear();
}

}