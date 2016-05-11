#ifndef _SPRITE2_SYMBOL_H_
#define _SPRITE2_SYMBOL_H_

#include <stddef.h>

namespace s2
{

class RenderParams;
class Sprite;

class Symbol
{
public:

	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const = 0;

}; // Symbol

}

#endif // _SPRITE2_SYMBOL_H_