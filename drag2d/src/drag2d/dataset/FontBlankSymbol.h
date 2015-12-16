#ifndef _DRAG2D_FONT_BLANK_SYMBOL_H_
#define _DRAG2D_FONT_BLANK_SYMBOL_H_

#include "ISymbol.h"
#include "render/ShapeStyle.h"

namespace d2d
{

class Font;
class FontBlankSprite;

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
	virtual void Draw(const Matrix& mt, const ColorTrans& col = ColorTrans(), 
		const ISprite* spr = NULL, const ISprite* root = NULL) const;
	virtual Rect GetSize(const ISprite* sprite = NULL) const;

	bool loadFont(const std::string& filename);

protected:
	virtual void LoadResources();

private:
	void DrawBackground(const FontBlankSprite* fb, const Matrix& mt) const;
	void DrawText(const FontBlankSprite* fb, const Matrix& mt) const;

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