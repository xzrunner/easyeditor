#include "Symbol.h"
#include "Sprite.h"

#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/StringHelper.h>
#include <ee/SymbolType.h>

#include <sprite2/RenderParams.h>
#include <sprite2/RVG.h>
#include <gum/trans_color.h>
#include <gum/GTxt.h>
#include <s2loader/TextboxLoader.h>
#include <gum/FilepathHelper.h>

#include <fstream>

namespace etext
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

s2::RenderReturn Symbol::DrawTree(cooking::DisplayList* dlist, const s2::RenderParams& rp, const s2::Sprite* spr) const
{
	if (!spr) {
		return s2::RENDER_NO_DATA;
	}

	const ee::SettingData& setting = ee::Config::Instance()->GetSettings();
	if (setting.visible_label_bg) {
		S2_MAT mt = spr->GetLocalMat() * rp.mt;
		DrawBackground(dynamic_cast<const Sprite*>(spr), mt);
	} 
 	if (setting.visible_label_text) {
		s2::TextboxSymbol::DrawTree(dlist, rp, spr);
 	}

	return s2::RENDER_OK;
}

bool Symbol::LoadResources()
{
	if (m_filepath == ee::SYM_TEXT_TAG) {
		return true;
	}
	if (!gum::FilepathHelper::Exists(m_filepath.c_str())) {
		return false;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	s2loader::TextboxLoader loader(GetTextbox());
	loader.LoadJson(value);

	return true;
}

void Symbol::DrawText(cooking::DisplayList* dlist, const gtxt_label_style& style, const S2_MAT& mt, const pt2::Color& mul,
	                  const pt2::Color& add, const CU_STR& text, int time, bool richtext) const
{
	std::string t_text = ee::StringHelper::ToUtf8(text.c_str());
	gum::GTxt::Instance()->Draw(dlist, style, mt, mul, add, t_text.c_str(), time, richtext);
}

void Symbol::DrawBackground(const Sprite* spr, const S2_MAT& mt) const
{
	if (!spr) {
		return;
	}

	s2::RVG::SetColor(pt2::Color(179, 179, 179, 179));

	const s2::Textbox& tb = spr->GetTextbox();
	float hw = tb.width * 0.5f,
		  hh = tb.height * 0.5f;

	sm::vec2 min(-hw, -hh), max(hw, hh);
	min = mt * min;
	max = mt * max;

	s2::RVG::Rect(nullptr, min, max, true);
}

}