#ifndef _SPRITE2_COMPLEX_SYMBOL_H_
#define _SPRITE2_COMPLEX_SYMBOL_H_

#include "Symbol.h"

#include <vector>

namespace s2
{

class ComplexSymbol : public Symbol
{
public:
	ComplexSymbol(void* ud);

	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;

	const std::vector<Sprite*>& GetChildren() const { return m_children; }

	/**
	 *  @note
	 *    api for dynamic change
	 */
	bool Add(Sprite* spr, int idx = -1);
	bool Remove(Sprite* spr);
	bool Clear();
	bool ResetOrder(const Sprite* spr, bool up);
	bool ResetOrderMost(const Sprite* spr, bool up);

private:
	std::vector<Sprite*> m_children;

}; // ComplexSymbol

}

#endif // _SPRITE2_COMPLEX_SYMBOL_H_