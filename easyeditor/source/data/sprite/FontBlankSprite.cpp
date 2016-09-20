#include "FontBlankSprite.h"
#include "FontBlankSymbol.h"
#include "FontBlankPropertySetting.h"

#include <sprite2/RenderColor.h>
#include <sprite2/DummySprite.h>
#include <gum/trans_color.h>

namespace ee
{

FontBlankSprite::FontBlankSprite()
	: font_color(0xffffffff)
{
	font		= "";
	has_edge	= false;
	align_hori	= HAT_LEFT;
	align_vert	= VAT_TOP;
	size		= 16;
	width		= 100;
	height		= 20;
}

FontBlankSprite::FontBlankSprite(const FontBlankSprite& spr)
	: s2::Sprite(spr)
	, s2::DummySprite(spr)
	, Sprite(spr)
{
	font		= spr.font;
	has_edge	= spr.has_edge;
	font_color	= spr.font_color;
	align_hori	= spr.align_hori;
	align_vert	= spr.align_vert;
	size		= spr.size;
	width		= spr.width;
	height		= spr.height;
	filename	= spr.filename;
	m_text		= spr.m_text;
	m_tid		= spr.m_tid;
}

FontBlankSprite& FontBlankSprite::operator = (const FontBlankSprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::DummySprite::operator = (spr);
	Sprite::operator = (spr);

	font		= spr.font;
	has_edge	= spr.has_edge;
	font_color	= spr.font_color;
	align_hori	= spr.align_hori;
	align_vert	= spr.align_vert;
	size		= spr.size;
	width		= spr.width;
	height		= spr.height;
	filename	= spr.filename;
	m_text		= spr.m_text;
	m_tid		= spr.m_tid;

	return *this;
}

FontBlankSprite::FontBlankSprite(FontBlankSymbol* sym)
	: s2::Sprite(sym)
	, s2::DummySprite(sym)
	, Sprite(sym)
{
	font = sym->font;
	font_color = gum::str2color(sym->color, gum::ARGB);

	if (sym->align_hori == 0)
		align_hori = HAT_LEFT;
	else if (sym->align_hori == 1)
		align_hori = HAT_RIGHT;
	else if (sym->align_hori == 2)
		align_hori = HAT_CENTER;
	else
		align_hori = HAT_AUTO;

	if (sym->align_vert == 0)
		align_vert = VAT_TOP;
	else if (sym->align_vert == 1)
		align_vert = VAT_BOTTOM;
	else if (sym->align_vert == 2)
		align_vert = VAT_CENTER;
	else
		align_vert = VAT_AUTO;

	align_vert = VAT_TOP;
	size	= static_cast<int>(sym->size);
	width	= static_cast<int>(sym->width);
	height	= static_cast<int>(sym->height);
	filename= sym->filename;
	has_edge= false;
}

void FontBlankSprite::Load(const Json::Value& val, const std::string& dir)
{
	Sprite::Load(val);

	FontBlankSymbol* sym = dynamic_cast<FontBlankSymbol*>(m_sym);

	// test if is old version
	if (val["font"].isNull())
	{
		font		= sym->font;
		font_color	= gum::str2color(sym->color, gum::ARGB);
		align_hori	= HoriAlignType((int)sym->align_hori);
		align_vert	= VertAlignType((int)sym->align_vert);
		size		= static_cast<int>(sym->size);
		width		= static_cast<int>(sym->width);
		height		= static_cast<int>(sym->height);
		filename	= sym->filename;
	}
	else
	{
		font		= val["font"].asString();
		font_color	= gum::str2color(val["color"].asString(), gum::ARGB);
		// is old version data
		if (!val["align"].isNull()) {
			align_hori = HoriAlignType(val["align"].asInt());
		} else {
			align_hori = HoriAlignType(val["align hori"].asInt());
			align_vert = VertAlignType(val["align vert"].asInt());
		}
		size		= val["size"].asInt();
		width		= val["label_width"].asInt();
		height		= val["label_height"].asInt();
		if (!val["font_filename"].isNull()) {
			filename = val["font_filename"].asString();
			sym->LoadFont(filename);
		}
		m_text		= val["content"].asString();
		m_tid		= val["tid"].asString();
	}
}

void FontBlankSprite::Store(Json::Value& val, const std::string& dir) const
{
	Sprite::Store(val);

	val["font"]			= font;
	val["color"]		= gum::color2str(font_color, gum::ARGB);
	val["align hori"]	= align_hori;
	val["align vert"]	= align_vert;
	val["size"]			= size;
	val["label_width"]	= width;
	val["label_height"] = height;
	val["font_filename"]= filename;
	val["content"]		= m_text;
	val["tid"]			= m_tid;
}

PropertySetting* FontBlankSprite::CreatePropertySetting(EditPanelImpl* stage)
{
	return new FontBlankPropertySetting(stage, this);
}

void FontBlankSprite::LoadFont(const std::string& _filename)
{
	FontBlankSymbol* sym = dynamic_cast<FontBlankSymbol*>(m_sym);
	if (sym->LoadFont(_filename)) {
		filename = _filename;
	}
}

const std::string& FontBlankSprite::GetSymbolName() const 
{ 
	const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(m_sym);
	return sym->name; 
}

}