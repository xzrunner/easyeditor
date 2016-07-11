#include "FontBlankSprite.h"
#include "SpriteFactory.h"
#include "BoundingBox.h"
#include "BBFactory.h"
#include "FontBlankPropertySetting.h"
#include "trans_color.h"

#include <sprite2/RenderColor.h>
#include <sprite2/DummySprite.h>

namespace ee
{

FontBlankSprite::FontBlankSprite()
	: m_symbol(NULL)
	, font_color(0xffffffff)
{
	m_core = new s2::DummySprite(this);

	font = "";
	has_edge = false;
	align_hori = HAT_LEFT;
	align_vert = VAT_TOP;
	size = 16;
	width = 100;
	height = 20;
}

FontBlankSprite::FontBlankSprite(const FontBlankSprite& sprite)
	: Sprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_core = new s2::DummySprite(*static_cast<s2::DummySprite*>(sprite.m_core), this);

	m_symbol->Retain();
	font = sprite.font;
	has_edge = sprite.has_edge;
	font_color = sprite.GetColor().mul;
	align_hori = sprite.align_hori;
	align_vert = sprite.align_vert;
	size = sprite.size;
	width = sprite.width;
	height = sprite.height;
	filename = sprite.filename;
}

FontBlankSprite::FontBlankSprite(FontBlankSymbol* symbol)
	: m_symbol(symbol)
{
	m_core = new s2::DummySprite(this);

	m_symbol->Retain();
	font = m_symbol->font;
	font_color = str2color(m_symbol->color, PT_ARGB);

	if (symbol->align_hori == 0)
		align_hori = HAT_LEFT;
	else if (symbol->align_hori == 1)
		align_hori = HAT_RIGHT;
	else if (symbol->align_hori == 2)
		align_hori = HAT_CENTER;
	else
		align_hori = HAT_AUTO;

	if (symbol->align_vert == 0)
		align_vert = VAT_TOP;
	else if (symbol->align_vert == 1)
		align_vert = VAT_BOTTOM;
	else if (symbol->align_vert == 2)
		align_vert = VAT_CENTER;
	else
		align_vert = VAT_AUTO;

	align_vert = VAT_TOP;
	size	= static_cast<int>(symbol->size);
	width	= static_cast<int>(symbol->width);
	height	= static_cast<int>(symbol->height);
	filename= symbol->filename;
	has_edge= false;

	BuildBounding();
}

FontBlankSprite::~FontBlankSprite()
{
	m_core->RemoveReference();

	if (m_symbol) {
		m_symbol->Release();
	}
}

FontBlankSprite* FontBlankSprite::Clone() const
{
	FontBlankSprite* sprite = new FontBlankSprite(*this);
	SpriteFactory::Instance()->Insert(sprite);
	return sprite;
}

const FontBlankSymbol& FontBlankSprite::GetSymbol() const
{
	return *m_symbol;
}

void FontBlankSprite::SetSymbol(Symbol* symbol)
{
	Sprite::SetSymbol(&m_symbol, symbol);
}

void FontBlankSprite::Load(const Json::Value& val, const std::string& dir)
{
	Sprite::Load(val);

	// test if is old version
	if (val["font"].isNull())
	{
		font = m_symbol->font;
		font_color = str2color(m_symbol->color, PT_ARGB);
		align_hori = HoriAlignType((int)m_symbol->align_hori);
		align_vert = VertAlignType((int)m_symbol->align_vert);
		size	= static_cast<int>(m_symbol->size);
		width	= static_cast<int>(m_symbol->width);
		height	= static_cast<int>(m_symbol->height);
		filename= m_symbol->filename;
	}
	else
	{
		font = val["font"].asString();
		font_color = str2color(val["color"].asString(), PT_ARGB);
		// is old version data
		if (!val["align"].isNull()) {
			align_hori = HoriAlignType(val["align"].asInt());
		} else {
			align_hori = HoriAlignType(val["align hori"].asInt());
			align_vert = VertAlignType(val["align vert"].asInt());
		}
		size = val["size"].asInt();
		width = val["label_width"].asInt();
		height = val["label_height"].asInt();
		if (!val["font_filename"].isNull()) {
			filename = val["font_filename"].asString();
			m_symbol->LoadFont(filename);
		}
		m_text = val["content"].asString();
		m_tid = val["tid"].asString();
	}

	BuildBounding();
}

void FontBlankSprite::Store(Json::Value& val, const std::string& dir) const
{
	Sprite::Store(val);

	val["font"] = font;
	val["color"] = color2str(font_color, PT_ARGB);
	val["align hori"] = align_hori;
	val["align vert"] = align_vert;
	val["size"] = size;
	val["label_width"] = width;
	val["label_height"] = height;
	val["font_filename"] = filename;
	val["content"] = m_text;
	val["tid"] = m_tid;
}

PropertySetting* FontBlankSprite::CreatePropertySetting(EditPanelImpl* stage)
{
	return new FontBlankPropertySetting(stage, this);
}

void FontBlankSprite::LoadFont(const std::string& _filename)
{
	if (m_symbol->LoadFont(_filename))
		filename = _filename;
}

}