#ifndef _EASYEDITOR_FONT_BLANK_SYMBOL_H_
#define _EASYEDITOR_FONT_BLANK_SYMBOL_H_

#include "Symbol.h"

#include <sprite2/DummySymbol.h>

namespace ee
{

class FontBlankSprite;

class FontBlankSymbol : public s2::DummySymbol, public Symbol
{
public:
	FontBlankSymbol();
	virtual ~FontBlankSymbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetSize(const Sprite* sprite = NULL) const;

	bool LoadFont(const std::string& filename);

protected:
	virtual void LoadResources();

private:
	void DrawBackground(const FontBlankSprite* fb, const sm::mat4& mt) const;
	void DrawText(const FontBlankSprite* fb, const sm::mat4& mt) const;

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