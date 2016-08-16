#ifndef _SPRITE2_TEXTBOX_SPRITE_H_
#define _SPRITE2_TEXTBOX_SPRITE_H_

#include "S2_Sprite.h"

#include <string>

namespace s2
{

class Textbox;

class TextboxSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	TextboxSprite();
	TextboxSprite(const TextboxSprite& spr);
	TextboxSprite(Symbol* sym) : Sprite(sym) {}

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual TextboxSprite* Clone() const;

	const Textbox* GetTextbox() const { return m_tb; }

	const std::string& GetText() const { return m_text; }
	
private:
	Textbox* m_tb;

	std::string m_text;

}; // TextboxSprite

}

#endif // _SPRITE2_TEXTBOX_SPRITE_H_