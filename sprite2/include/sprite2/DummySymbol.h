#ifndef _SPRITE2_DUMMY_SYMBOL_H_
#define _SPRITE2_DUMMY_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

namespace s2
{

class DummySymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	DummySymbol() {}

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const {}
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const { return sm::rect(0, 0); }

}; // DummySymbol

}

#endif // _SPRITE2_DUMMY_SYMBOL_H_