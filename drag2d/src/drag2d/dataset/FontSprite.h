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
	virtual bool Update(int version) { return false; }
	virtual const FontBlankSymbol& GetSymbol() const;
	virtual void SetSymbol(ISymbol* symbol);

	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;

	virtual void BuildBounding();

	virtual IPropertySetting* CreatePropertySetting(EditPanelImpl* stage);

	void loadFont(const std::string& filename);

	void SetTextContent(const wxString& str) { m_text = str; }
	const wxString& GetTextContext() const { return m_text; }

	void SetTextID(const std::string& tid) { m_tid = tid; }
	const std::string& GetTextID() const { return m_tid; }

	const std::string& GetSymbolName() const { return m_symbol->name; }

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