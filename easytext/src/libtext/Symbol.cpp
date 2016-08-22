#include "Symbol.h"
#include "Sprite.h"

#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/EE_GTxt.h>
#include <ee/trans_color.h>
#include <ee/StringHelper.h>

#include <sprite2/RenderParams.h>
#include <sprite2/S2_RVG.h>

#include <gtxt.h>

#include <fstream>

namespace etext
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

void Symbol::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	s2::RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;
	}
	const ee::SettingData& setting = ee::Config::Instance()->GetSettings();
	if (setting.visible_label_bg) {
		DrawBackground(dynamic_cast<const Sprite*>(spr), p.mt);
	} 
 	if (setting.visible_label_text) {
 		DrawText(dynamic_cast<const Sprite*>(spr), p);
 	}
}

void Symbol::LoadResources()
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	m_tb.width			= value["width"].asInt();
	m_tb.height			= value["height"].asInt();

	m_tb.font_type		= value["font"].asInt();
	m_tb.font_size		= value["font_size"].asInt();
	m_tb.font_color		= str2color(value["font_color"].asString(), ee::PT_RGBA);

	m_tb.has_edge		= value["edge"].asBool();
	m_tb.edge_size		= static_cast<float>(value["edge_size"].asDouble());
	m_tb.edge_color		= str2color(value["edge_color"].asString(), ee::PT_RGBA);

	m_tb.space_hori		= static_cast<float>(value["space_hori"].asDouble());
	m_tb.space_vert		= static_cast<float>(value["space_vert"].asDouble());

	m_tb.align_hori		= (s2::Textbox::HoriAlign)(value["align_hori"].asInt());
	m_tb.align_vert		= (s2::Textbox::VertAlign)(value["align_vert"].asInt());

	m_tb.overflow = true;
	if (!value["overflow"].isNull()) {
		m_tb.overflow	= value["overflow"].asBool();
	}

	m_tb.richtext = true;
	if (!value["richtext"].isNull()) {
		m_tb.richtext	= value["richtext"].asBool();
	}
}

void Symbol::DrawBackground(const Sprite* spr, const sm::mat4& mt) const
{
	if (!spr) {
		return;
	}

	s2::RVG::SetColor(s2::Color(179, 179, 179, 179));

	const s2::Textbox& tb = spr->GetTextbox();
	float hw = tb.width * 0.5f,
		  hh = tb.height * 0.5f;

	sm::vec2 min(-hw, -hh), max(hw, hh);
	min = mt * min;
	max = mt * max;

	s2::RVG::Rect(min, max, true);
}

void Symbol::DrawText(const Sprite* spr, const s2::RenderParams& params) const
{
	// todo
//	s2::TextboxSymbol::Draw(params, spr);

	//////////////////////////////////////////////////////////////////////////

	if (!spr) {
		return;
	}

	const s2::TextboxSprite* tb_spr = dynamic_cast<const s2::TextboxSprite*>(spr); 	
	const std::string& text = tb_spr->GetText();
	if (text.empty()) {
		return;
	}

	const s2::Textbox& tb = tb_spr->GetTextbox();

	gtxt_label_style s;

	s.width			= tb.width;
	s.height		= tb.height;

	s.gs.font		= tb.font_type;
	s.gs.font_size	= tb.font_size;
	s.gs.font_color.integer = tb.font_color.ToRGBA();

	s.gs.edge		= tb.has_edge;
	s.gs.edge_size	= tb.edge_size;
	s.gs.edge_color.integer = tb.edge_color.ToRGBA();

	s.align_h		= tb.align_hori;
	s.align_v		= tb.align_vert;

	s.space_h		= tb.space_hori;
	s.space_v		= tb.space_vert;

	std::string t_text = ee::StringHelper::ToUtf8(text);
	ee::GTxt::Instance()->Draw(s, params.mt, params.color.mul, params.color.add, t_text, tb_spr->GetTime(), tb.richtext);

	tb_spr->UpdateTime();
}

}