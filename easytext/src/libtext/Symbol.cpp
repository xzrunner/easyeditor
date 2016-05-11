#include "Symbol.h"
#include "Sprite.h"

#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/EE_RVG.h>
#include <ee/EE_GTxt.h>
#include <ee/Math2D.h>

#include <sprite2/RenderParams.h>

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

void Symbol::Draw(const s2::RenderParams& params, const ee::Sprite* spr) const
{
	const ee::SettingData& setting = ee::Config::Instance()->GetSettings();
	if (setting.visible_label_bg) {
		DrawBackground(spr, params.mt);
	} 
 	if (setting.visible_label_text) {
 		DrawText(spr, params);
 	}
}

ee::Rect Symbol::GetSize(const ee::Sprite* sprite) const
{
	if (sprite) {
		const Sprite* font = static_cast<const Sprite*>(sprite);
		int w, h;
		font->GetSize(w, h);
		return ee::Rect(static_cast<float>(w), static_cast<float>(h));
	} else {
		return ee::Rect(static_cast<float>(m_width), static_cast<float>(m_height));
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
}

void Symbol::DrawBackground(const ee::Sprite* sprite, const sm::mat4& mt) const
{
	if (!sprite) {
		return;
	}

	if (const Sprite* font = dynamic_cast<const Sprite*>(sprite)) {
		ee::RVG::Color(s2::Color(179, 179, 179, 179));

		int w, h;
		font->GetSize(w, h);
		float hw = w * 0.5f,
			hh = h * 0.5f;

		sm::vec2 min(-hw, -hh), max(hw, hh);
		min = ee::Math2D::TransVector(min, mt);
		max = ee::Math2D::TransVector(max, mt);

		ee::RVG::Rect(min, max, true);
	}
}

void Symbol::DrawText(const ee::Sprite* sprite, const s2::RenderParams& params) const
{
	if (!sprite) {
		return;
	}

	if (const Sprite* font = dynamic_cast<const Sprite*>(sprite)) {
		if (font->GetText().empty()) {
			return;
		}

		gtxt_label_style style;

		font->GetSize(style.width, style.height);
		font->GetAlign(style.align_h, style.align_v);
		font->GetSpace(style.space_h, style.space_v);

		style.gs.font = font->GetFont();
		style.gs.font_size = font->GetFontSize();
		style.gs.font_color.integer = font->GetFontColor().ToRGBA();

		style.gs.edge = font->GetEdge();
		style.gs.edge_size = font->GetEdgeSize();
		style.gs.edge_color.integer = font->GetEdgeColor().ToRGBA();

		ee::GTxt::Instance()->Draw(style, params.mt, params.color.mul, params.color.add, font->GetText(), font->GetTime());
		font->UpdateTime();
	}
}

}