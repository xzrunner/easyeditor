#include "Sprite.h"
#include "Symbol.h"
#include "PropertySetting.h"

#include <ee/SpriteFactory.h>
#include <ee/trans_color.h>

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

	m_tb.width			= text_val["width"].asInt();
	m_tb.height			= text_val["height"].asInt();

	m_tb.font_type		= text_val["font"].asInt();
	m_tb.font_size		= text_val["font_size"].asInt();
	m_tb.font_color		= str2color(text_val["font_color"].asString(), ee::PT_RGBA);

	m_tb.has_edge		= text_val["edge"].asBool();
	m_tb.edge_size		= text_val["edge_size"].asInt();
	m_tb.edge_color		= str2color(text_val["edge_color"].asString(), ee::PT_RGBA);

	m_tb.align_hori		= s2::Textbox::HoriAlign(text_val["align_hori"].asInt());
	m_tb.align_vert		= s2::Textbox::VertAlign(text_val["align_vert"].asInt());

	m_tb.space_hori		= text_val["space_hori"].asDouble();
	m_tb.space_vert		= text_val["space_vert"].asDouble();

	m_text				= text_val["text"].asString();
	m_tid				= text_val["tid"].asString();

	m_tb.overflow = true;
	if (!text_val["overflow"].isNull()) {
		m_tb.overflow	= text_val["overflow"].asBool();
	}

	m_tb.richtext = true;
	if (!text_val["richtext"].isNull()) {
		m_tb.richtext	= text_val["richtext"].asBool();
	}
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	Json::Value text_val;

	text_val["width"]			= m_tb.width;
	text_val["height"]			= m_tb.height;

	text_val["font"]			= m_tb.font_type;
	text_val["font_size"]		= m_tb.font_size;
	text_val["font_color"]		= color2str(m_tb.font_color, ee::PT_RGBA);

	text_val["edge"]			= m_tb.has_edge;
	text_val["edge_size"]		= m_tb.edge_size;
	text_val["edge_color"]		= color2str(m_tb.edge_color, ee::PT_RGBA);

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