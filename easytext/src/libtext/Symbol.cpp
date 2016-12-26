#include "Symbol.h"
#include "Sprite.h"

#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/StringHelper.h>

#include <sprite2/RenderParams.h>
#include <sprite2/S2_RVG.h>
#include <sprite2/RenderFilter.h>
#include <gum/trans_color.h>
#include <gum/GUM_GTxt.h>
#include <gum/TextboxLoader.h>

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
	if (!spr) {
		return;
	}

	const ee::SettingData& setting = ee::Config::Instance()->GetSettings();
	if (setting.visible_label_bg) {
		sm::mat4 mt = spr->GetLocalMat() * params.mt;
		DrawBackground(dynamic_cast<const Sprite*>(spr), mt);
	} 
 	if (setting.visible_label_text) {
		s2::TextboxSymbol::Draw(params, spr);
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

	gum::TextboxLoader loader(GetTextbox());
	loader.LoadJson(value);
}

void Symbol::DrawText(const gtxt_label_style& style, const sm::mat4& mt, const s2::Color& mul, 
					  const s2::Color& add, const std::string& text, int time, bool richtext) const
{
	std::string t_text = ee::StringHelper::ToUtf8(text);
	gum::GTxt::Instance()->Draw(style, mt, mul, add, t_text, time, richtext);
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

}