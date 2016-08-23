#include "TextboxSprite.h"
#include "TextboxSymbol.h"

#include <stddef.h>
#include <assert.h>

namespace s2
{

TextboxSprite::TextboxSprite()
	: m_time(0)
{
}

TextboxSprite::TextboxSprite(Symbol* sym) 
	: Sprite(sym) 
	, m_time(0)
{
	TextboxSymbol* tb_sym = VI_DOWNCASTING<TextboxSymbol*>(sym);
	assert(tb_sym);
	m_tb = tb_sym->GetTextbox();
}

TextboxSprite* TextboxSprite::Clone() const
{
	return new TextboxSprite(*this);
}

}