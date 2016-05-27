#ifndef _SPRITE2_TEXTBOX_SYMBOL_H_
#define _SPRITE2_TEXTBOX_SYMBOL_H_

#include "S2_Symbol.h"
#include "Textbox.h"

#include <stdint.h>

namespace s2
{

class TextboxSymbol : public Symbol
{
public:
	TextboxSymbol(void* ud, Textbox tb);

	virtual void Draw(const RenderParams& params, const Sprite* spr) const;

private:
	Textbox m_tb;

}; // TextboxSymbol

}

#endif // _SPRITE2_TEXTBOX_SYMBOL_H_