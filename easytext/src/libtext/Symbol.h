#ifndef _EASYTEXT_SYMBOL_H_
#define _EASYTEXT_SYMBOL_H_

#include <drag2d.h>

#include "typedef.h"

namespace etext
{

class Symbol : public d2d::ISymbol
{
public:
	Symbol();
	virtual ~Symbol();

	//
	// ICloneable interface
	//
	virtual Symbol* Clone() const { return NULL; }

	//
	// ISymbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::ISprite* spr = NULL, const d2d::ISprite* root = NULL) const;
	virtual d2d::Rect GetSize(const d2d::ISprite* sprite = NULL) const;

	static d2d::ISymbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void DrawBackground(const d2d::ISprite* sprite, const d2d::Matrix& mt) const;
	void DrawText(const d2d::ISprite* sprite, const d2d::Matrix& mt, const d2d::Colorf& mul, const d2d::Colorf& add) const;

private:
	int m_width;
	int m_height;

	int m_font;
	int m_font_size;
	std::string m_font_color;

	bool m_edge;
	float m_edge_size;
	std::string m_edge_color;

	HoriAlignType m_align_hori;
	VertAlignType m_align_vert;

	float m_space_hori;
	float m_space_vert;

private:
	d2d::ShapeStyle m_bg_style;

	friend class Sprite;

}; // Symbol

}

#endif // _EASYTEXT_SYMBOL_H_