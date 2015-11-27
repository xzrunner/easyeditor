#ifndef _DRAG2D_FONT_BLANK_SYMBOL_H_
#define _DRAG2D_FONT_BLANK_SYMBOL_H_

#include "ISymbol.h"
#include "render/ShapeStyle.h"

namespace d2d
{

class Font;

class FontBlankSymbol : public ISymbol
{
public:
	FontBlankSymbol();
	virtual ~FontBlankSymbol();

	//
	// ICloneable interface
	//
	virtual FontBlankSymbol* Clone() const { return NULL; }

	//
	// ISymbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const Matrix& mt,
		const Colorf& mul = Colorf(1, 1, 1, 1), 
		const Colorf& add = Colorf(0, 0, 0, 0),
		const Colorf& r_trans = Colorf(1, 0, 0, 0),
		const Colorf& g_trans = Colorf(0, 1, 0, 0),
		const Colorf& b_trans = Colorf(0, 0, 1, 0),
		const ISprite* sprite = NULL) const;
	virtual Rect GetSize(const ISprite* sprite = NULL) const;

	bool loadFont(const std::string& filename);

protected:
	virtual void LoadResources();

private:
	void DrawBackground(const ISprite* sprite, const Matrix& mt) const;
	void DrawText(const ISprite* sprite, const Matrix& mt) const;

	// from NeHe's FreeType
	void print(float x, float y, const char* text) const;

public:
	std::string font;

	std::string color;

	float align_hori, align_vert;
	float size;

	float width, height;

	std::string filename;

private:
	Font* m_font;

	ShapeStyle m_style;

}; // FontBlankSymbol

}

#endif // _DRAG2D_FONT_BLANK_SYMBOL_H_