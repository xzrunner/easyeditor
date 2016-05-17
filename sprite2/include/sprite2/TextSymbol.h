#ifndef _SPRITE2_TEXT_SYMBOL_H_
#define _SPRITE2_TEXT_SYMBOL_H_

#include "Symbol.h"

#include <stdint.h>

namespace s2
{

class TextSymbol : public Symbol
{
public:
	TextSymbol(void* ud);

	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;

}; // TextSymbol

}

#endif // _SPRITE2_TEXT_SYMBOL_H_