#ifndef _SPRITE2_SCALE9_SYMBOL_H_
#define _SPRITE2_SCALE9_SYMBOL_H_

#include "Symbol.h"

namespace s2
{

class Scale9Symbol : public Symbol
{
public:
	Scale9Symbol(void* ud);

	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;

}; // Scale9Symbol

}

#endif // _SPRITE2_SCALE9_SYMBOL_H_	