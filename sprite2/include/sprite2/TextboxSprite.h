#ifndef _SPRITE2_TEXTBOX_SPRITE_H_
#define _SPRITE2_TEXTBOX_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class Textbox;

class TextboxSprite : public Sprite
{
public:
	TextboxSprite(void* ud);
	
	virtual bool Update(float dt) { return false; }

	const Textbox* GetTextbox() const { return m_tb; }

	const std::string& GetText() const { return m_text; }
	
private:
	TextboxSprite() {}
	const TextboxSprite& operator = (const TextboxSprite& spr) { return *this; }

private:
	Textbox* m_tb;

	std::string m_text;

}; // TextboxSprite

}

#endif // _SPRITE2_TEXTBOX_SPRITE_H_