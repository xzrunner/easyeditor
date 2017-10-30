#ifndef _EASYEDITOR_FONT_BLANK_SYMBOL_H_
#define _EASYEDITOR_FONT_BLANK_SYMBOL_H_

#include "Symbol.h"

#include S2_MAT_HEADER

namespace ee
{

class FontBlankSprite;

class FontBlankSymbol : public Symbol
{
public:
	FontBlankSymbol();
	virtual ~FontBlankSymbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const s2::SymbolVisitor& visitor) {}
	virtual s2::RenderReturn DrawTree(cooking::DisplayList* dlist, const s2::RenderParams& rp, const s2::Sprite* spr = nullptr) const;

	bool LoadFont(const std::string& filename);

protected:
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const;

	virtual bool LoadResources();

private:
	void DrawBackground(const FontBlankSprite* fb, const S2_MAT& mt) const;
	void DrawText(const FontBlankSprite* fb, const S2_MAT& mt) const;

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