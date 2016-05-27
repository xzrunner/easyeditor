#ifndef _SPRITE2_TEXTURE_SYMBOL_H_
#define _SPRITE2_TEXTURE_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

namespace s2
{

class TextureSymbol : public Symbol
{
public:
	TextureSymbol(void* ud);

	virtual void Draw(const RenderParams& params, const Sprite* spr) const;

}; // TextureSymbol

}

#endif // _SPRITE2_TEXTURE_SYMBOL_H_