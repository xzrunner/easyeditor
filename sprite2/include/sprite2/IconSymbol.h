#ifndef _SPRITE2_ICON_SYMBOL_H_
#define _SPRITE2_ICON_SYMBOL_H_

#include "Symbol.h"

#include <stdint.h>

namespace s2
{

class IconSymbol : public Symbol
{
public:
	IconSymbol(void* ud);

	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;

}; // IconSymbol

}

#endif // _SPRITE2_ICON_SYMBOL_H_