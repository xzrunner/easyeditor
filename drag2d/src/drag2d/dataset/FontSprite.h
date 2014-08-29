#ifndef D2D_FONT_SPRITE_H
#define D2D_FONT_SPRITE_H

#include "ISprite.h"

#include "FontBlankSymbol.h"

namespace d2d
{

enum HoriAlignType
{
	HAT_LEFT = 0,
	HAT_RIGHT = 1,
	HAT_CENTER = 2,
	HAT_AUTO = 3
};

enum VertAlignType
{
	VAT_TOP = 0,
	VAT_BOTTOM = 1,
	VAT_CENTER = 2,
	VAT_AUTO = 3
};

class FontSprite : public ISprite
{
public:
	FontSprite();
	FontSprite(const FontSprite& sprite);
	FontSprite(FontBlankSymbol* symbol);
	virtual ~FontSprite();

	//
	// ICloneable interface
	//
	virtual FontSprite* clone() const;

	//
	// ISprite interface
	//
	virtual const FontBlankSymbol& getSymbol() const;
	virtual void setSymbol(ISymbol* symbol);
	virtual void loadBodyFromFile();

	virtual void load(const Json::Value& val);
	virtual void store(Json::Value& val) const;

	virtual void buildBounding();

	void loadFont(const std::string& filename);

	void SetTextContent(const std::string& str) { m_text = str; }
	const std::string& GetTextContext() const { return m_text; }

protected:
	FontBlankSymbol* m_symbol;

public:
	std::string font;

	bool has_edge;

	Colorf color;

	HoriAlignType align_hori;
	VertAlignType align_vert;
	int size;

	int width, height;

	std::string filename;

private:
	std::string m_text;

}; // FontSprite
}

#endif // D2D_FONT_SPRITE_H