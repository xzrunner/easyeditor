#ifndef _EASYEDITOR_FONT_BLANK_SYMBOL_H_
#define _EASYEDITOR_FONT_BLANK_SYMBOL_H_

#include "Symbol.h"

namespace ee
{

class FontBlankSprite;

class FontBlankSymbol : public Symbol
{
public:
	FontBlankSymbol();
	virtual ~FontBlankSymbol();

	//
	// Cloneable interface
	//
	virtual FontBlankSymbol* Clone() const { return NULL; }

	//
	// Symbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const Matrix& mt, const ColorTrans& col = ColorTrans(), 
		const Sprite* spr = NULL, const Sprite* root = NULL) const;
	virtual Rect GetSize(const Sprite* sprite = NULL) const;

	bool LoadFont(const std::string& filename);

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

}; // FontBlankSymbol

}

#endif // _EASYEDITOR_FONT_BLANK_SYMBOL_H_