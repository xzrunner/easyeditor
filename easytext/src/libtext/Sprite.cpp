#include "Sprite.h"
#include "Symbol.h"
#include "PropertySetting.h"

#include <ee/SpriteFactory.h>

#include <gum/trans_color.h>
#include <gum/TextboxLoader.h>

namespace etext
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::TextboxSprite(spr)
	, ee::Sprite(spr)
{
	m_tid = spr.m_tid;
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::TextboxSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	m_tid = spr.m_tid;
	return *this;
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::TextboxSprite(sym)
	, ee::Sprite(sym)
{
}

bool Sprite::Update(const s2::RenderParams& params) 
{ 
	return m_text.find("dynamic") != std::string::npos;
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	const Json::Value& text_val = val["text"];

	m_text	= text_val["text"].asString();
	m_tid	= text_val["tid"].asString();

	gum::TextboxLoader loader(m_tb);
	loader.LoadJson(text_val);
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	Json::Value text_val;

	text_val["width"]			= m_tb.width;
	text_val["height"]			= m_tb.height;

	text_val["font"]			= m_tb.font_type;
	text_val["font_size"]		= m_tb.font_size;
	text_val["font_color"]		= color2str(m_tb.font_color, gum::RGBA);

	text_val["edge"]			= m_tb.has_edge;
	text_val["edge_size"]		= m_tb.edge_size;
	text_val["edge_color"]		= color2str(m_tb.edge_color, gum::RGBA);

	text_val["align_hori"]		= m_tb.align_hori;
	text_val["align_vert"]		= m_tb.align_vert;

	text_val["space_hori"]		= m_tb.space_hori;
	text_val["space_vert"]		= m_tb.space_vert;	

	text_val["text"]			= m_text;
	text_val["tid"]				= m_tid;

	text_val["overflow"]		= m_tb.overflow;

	text_val["richtext"]		= m_tb.richtext;

	val["text"]					= text_val;
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new PropertySetting(stage, this);
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

}