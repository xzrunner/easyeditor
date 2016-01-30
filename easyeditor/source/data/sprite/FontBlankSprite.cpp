#include "FontBlankSprite.h"
#include "SpriteFactory.h"
#include "Rect.h"
#include "BoundingBox.h"
#include "BBFactory.h"
#include "trans_color.h"

//#include "view/FontBlankPropertySetting.h"

namespace ee
{

FontBlankSprite::FontBlankSprite()
	: m_symbol(NULL)
{
	font = "";
	has_edge = false;
	font_color.Set(1, 1, 1, 1);
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
	m_symbol->Retain();
	font = sprite.font;
	has_edge = sprite.has_edge;
	color = sprite.color;
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
	m_symbol->Retain();
	font = m_symbol->font;
	font_color = TransColor(m_symbol->color, PT_ARGB);

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
	size = symbol->size;
	width = symbol->width;
	height = symbol->height;
	filename = symbol->filename;
	has_edge = false;

	BuildBounding();
}

FontBlankSprite::~FontBlankSprite()
{
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

void FontBlankSprite::Load(const Json::Value& val)
{
	Sprite::Load(val);

	// test if is old version
	if (val["font"].isNull())
	{
		font = m_symbol->font;
		font_color = TransColor(m_symbol->color, PT_ARGB);
		align_hori = HoriAlignType((int)m_symbol->align_hori);
		align_vert = VertAlignType((int)m_symbol->align_vert);
		size = m_symbol->size;
		width = m_symbol->width;
		height = m_symbol->height;
		filename = m_symbol->filename;
	}
	else
	{
		font = val["font"].asString();
		font_color = TransColor(val["color"].asString(), PT_ARGB);
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

void FontBlankSprite::Store(Json::Value& val) const
{
	Sprite::Store(val);

	val["font"] = font;
	val["color"] = TransColor(font_color, PT_ARGB);
	val["align hori"] = align_hori;
	val["align vert"] = align_vert;
	val["size"] = size;
	val["label_width"] = width;
	val["label_height"] = height;
	val["font_filename"] = filename;
	val["content"] = m_text;
	val["tid"] = m_tid;
}

void FontBlankSprite::BuildBounding()
{
	if (!m_bounding) {
		m_bounding = BBFactory::CreateBB(e_obb);
	}
	Rect rect(width, height);
	if (m_offset.x == 0 && m_offset.y == 0) {
		m_offset.Set(rect.CenterX(), rect.CenterY());
	}
	rect.Scale(m_scale.x, m_scale.y);
	rect.Shear(m_shear.x, m_shear.y);
	m_bounding->InitFromRect(rect);
	m_bounding->SetTransform(m_pos, m_offset, m_angle);
}

IPropertySetting* FontBlankSprite::CreatePropertySetting(EditPanelImpl* stage)
{
	return new FontBlankPropertySetting(stage, this);
}

void FontBlankSprite::LoadFont(const std::string& _filename)
{
	if (m_symbol->LoadFont(_filename))
		filename = _filename;
}

}