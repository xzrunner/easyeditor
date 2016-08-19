#ifndef _SPRITE2_TEXTBOX_SPRITE_H_
#define _SPRITE2_TEXTBOX_SPRITE_H_

#include "S2_Sprite.h"
#include "Textbox.h"

#include <string>

namespace s2
{

class TextboxSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	TextboxSprite();
	TextboxSprite(Symbol* sym);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual TextboxSprite* Clone() const;

	const Textbox& GetTextbox() const { return m_tb; }
	Textbox& GetTextbox() { return m_tb; }

	const std::string& GetText() const { return m_text; }

	int GetTime() const { return m_time; }
	void UpdateTime() const { ++m_time; }

	VI_DUMMY_FUNC

protected:
	Textbox m_tb;

	std::string m_text;

private:
	// for dynamic draw
	mutable int m_time;	

}; // TextboxSprite

}

#endif // _SPRITE2_TEXTBOX_SPRITE_H_