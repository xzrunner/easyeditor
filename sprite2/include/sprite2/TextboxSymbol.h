#ifndef _SPRITE2_TEXTBOX_SYMBOL_H_
#define _SPRITE2_TEXTBOX_SYMBOL_H_

#include "S2_Symbol.h"
#include "Textbox.h"

#include <stdint.h>

namespace s2
{

class TextboxSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	TextboxSymbol() {}
	TextboxSymbol(Textbox tb);

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	const Textbox& GetTextbox() const { return m_tb; }
	Textbox& GetTextbox() { return m_tb; }

protected:
	Textbox m_tb;

}; // TextboxSymbol

}

#endif // _SPRITE2_TEXTBOX_SYMBOL_H_