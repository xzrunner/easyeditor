#ifndef D2D_FONT_SPRITE_H
#define D2D_FONT_SPRITE_H

#include "ISprite.h"

#include "FontBlankSymbol.h"

namespace d2d
{
enum AlignType
{
	AT_LEFT = 0,
	AT_RIGHT = 1,
	AT_CENTER = 2
};

class FontSprite : public ISprite
{
public:
	FontSprite();
	FontSprite(const FontSprite& sprite);
	FontSprite(FontBlankSymbol* symbol);
	virtual ~FontSprite();

	//
	// IObject interface
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

protected:
	FontBlankSymbol* m_symbol;

public:
	std::string font;

	Colorf color;

	AlignType align;
	int size;

	int width, height;

	std::string filename;

}; // FontSprite
}

#endif // D2D_FONT_SPRITE_H