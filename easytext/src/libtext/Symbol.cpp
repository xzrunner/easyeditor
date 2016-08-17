#include "Symbol.h"
#include "Sprite.h"

#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/EE_GTxt.h>
#include <ee/Math2D.h>

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
		p.mt = dynamic_cast<const ee::Sprite*>(spr)->GetTransMatrix() * params.mt;
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

sm::rect Symbol::GetBounding(const s2::Sprite* spr) const
{
	if (spr) {
		const Sprite* font = dynamic_cast<const Sprite*>(spr);
		int w, h;
		font->GetSize(w, h);
		return sm::rect(static_cast<float>(w), static_cast<float>(h));
	} else {
		return sm::rect(static_cast<float>(m_width), static_cast<float>(m_height));
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

	m_width = value["width"].asInt();
	m_height = value["height"].asInt();

	m_font = value["font"].asInt();
	m_font_size = value["font_size"].asInt();
	m_font_color = value["font_color"].asString();

	m_edge = value["edge"].asBool();
	m_edge_size = static_cast<float>(value["edge_size"].asDouble());
	m_edge_color = value["edge_color"].asString();

	m_space_hori = static_cast<float>(value["space_hori"].asDouble());
	m_space_vert = static_cast<float>(value["space_vert"].asDouble());

	m_align_hori = (HoriAlignType)(value["align_hori"].asInt());
	m_align_vert = (VertAlignType)(value["align_vert"].asInt());

	m_overflow = true;
	if (!value["overflow"].isNull()) {
		m_overflow = value["overflow"].asBool();
	}

	m_richtext = true;
	if (!value["richtext"].isNull()) {
		m_richtext = value["richtext"].asBool();
	}
}

void Symbol::DrawBackground(const Sprite* spr, const sm::mat4& mt) const
{
	if (!spr) {
		return;
	}

	s2::RVG::SetColor(s2::Color(179, 179, 179, 179));

	int w, h;
	spr->GetSize(w, h);
	float hw = w * 0.5f,
		  hh = h * 0.5f;

	sm::vec2 min(-hw, -hh), max(hw, hh);
	min = ee::Math2D::TransVector(min, mt);
	max = ee::Math2D::TransVector(max, mt);

	s2::RVG::Rect(min, max, true);
}

void Symbol::DrawText(const Sprite* spr, const s2::RenderParams& params) const
{
	if (!spr) {
		return;
	}
	if (spr->GetText().empty()) {
		return;
	}

	gtxt_label_style style;

	spr->GetSize(style.width, style.height);
	spr->GetAlign(style.align_h, style.align_v);
	spr->GetSpace(style.space_h, style.space_v);

	style.overflow = spr->GetOverflow();

	style.gs.font = spr->GetFont();
	style.gs.font_size = spr->GetFontSize();
	style.gs.font_color.integer = spr->GetFontColor().ToRGBA();

	style.gs.edge = spr->GetEdge();
	style.gs.edge_size = spr->GetEdgeSize();
	style.gs.edge_color.integer = spr->GetEdgeColor().ToRGBA();

	ee::GTxt::Instance()->Draw(style, params.mt, params.color.mul, params.color.add, spr->GetText(), spr->GetTime(), spr->GetRichtext());
	spr->UpdateTime();
}

}