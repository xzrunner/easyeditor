#ifndef _EASYEDITOR_FONT_BLANK_SPRITE_H_
#define _EASYEDITOR_FONT_BLANK_SPRITE_H_

#include "Sprite.h"

#include <painting2/Color.h>

namespace ee
{

class FontBlankSymbol;

class FontBlankSprite : public Sprite
{
public:
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

public:
	FontBlankSprite();
	FontBlankSprite(const FontBlankSprite& spr);
	FontBlankSprite& operator = (const FontBlankSprite& spr);
	FontBlankSprite(const s2::SymPtr& sym, uint32_t id = -1);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual void Load(const Json::Value& val, const std::string& dir = "") override;
	virtual void Store(Json::Value& val, const std::string& dir = "") const override;

	virtual PropertySetting* CreatePropertySetting(EditPanelImpl* stage) override;

	void LoadFont(const std::string& filename);

	void SetTextContent(const std::string& str) { m_text = str; }
	const std::string& GetTextContext() const { return m_text; }

	void SetTextID(const std::string& tid) { m_tid = tid; }
	const std::string& GetTextID() const { return m_tid; }

	const std::string& GetSymbolName() const;

public:
	std::string font;

	bool has_edge;

	pt2::Color font_color;

	HoriAlignType align_hori;
	VertAlignType align_vert;
	int size;

	int width, height;

	std::string filename;

private:
	std::string m_text;

	std::string m_tid;

	S2_SPR_CLONE_FUNC(FontBlankSprite)

}; // FontBlankSprite
}

#endif // _EASYEDITOR_FONT_BLANK_SPRITE_H_