#ifndef D2D_FONT_SPRITE_H
#define D2D_FONT_SPRITE_H

#include "ISprite.h"

#include "FontBlankSymbol.h"

#include <common/types.h>

namespace d2d
{

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
	virtual FontSprite* Clone() const;

	//
	// ISprite interface
	//
	virtual const FontBlankSymbol& getSymbol() const;
	virtual void setSymbol(ISymbol* symbol);

	virtual void load(const Json::Value& val);
	virtual void store(Json::Value& val) const;

	virtual void buildBounding();

	virtual IPropertySetting* CreatePropertySetting(EditPanel* stage);

	void loadFont(const std::string& filename);

	void SetTextContent(const wxString& str) { m_text = str; }
	const wxString& GetTextContext() const { return m_text; }

	void SetTextID(const std::string& tid) { m_tid = tid; }
	const std::string& GetTextID() const { return m_tid; }

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
	wxString m_text;

	std::string m_tid;

}; // FontSprite
}

#endif // D2D_FONT_SPRITE_H