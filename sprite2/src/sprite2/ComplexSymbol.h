#ifndef _SPRITE2_COMPLEX_SYMBOL_H_
#define _SPRITE2_COMPLEX_SYMBOL_H_

#include "Symbol.h"

#include <vector>

namespace s2
{

class ComplexSymbol : public Symbol
{
public:
	ComplexSymbol();

	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;

	const std::vector<Sprite*>& GetChildren() const { return m_children; }

private:
	std::vector<Sprite*> m_children;

}; // ComplexSymbol

}

#endif // _SPRITE2_COMPLEX_SYMBOL_H_