#ifndef _EASYTEXT_SPRITE_H_
#define _EASYTEXT_SPRITE_H_

#include "Symbol.h"

namespace etext
{

class Sprite : public d2d::ISprite
{
public:
	Sprite();
	Sprite(const Sprite& sprite);
	Sprite(Symbol* symbol);
	virtual ~Sprite();

	//
	// ICloneable interface
	//
	virtual Sprite* Clone() const;

	//
	// ISprite interface
	//
	virtual bool Update(int version) { return false; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(d2d::ISymbol* symbol);

	virtual d2d::IPropertySetting* CreatePropertySetting(d2d::EditPanelImpl* stage);

	static d2d::ISprite* Create(d2d::ISymbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	int m_width;
	int m_height;

	std::string m_font;
	int m_font_size;

	bool m_has_edge;

	d2d::Colorf m_color;

	HoriAlignType m_align_hori;
	VertAlignType m_align_vert;

private:
	std::string m_text;
	std::string m_tid;

private:
	Symbol* m_symbol;

	friend class Symbol;
	friend class PropertySetting;

}; // Sprite

}

#endif // _EASYTEXT_SPRITE_H_