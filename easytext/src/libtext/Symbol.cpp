#include "Symbol.h"
#include "Sprite.h"

#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/StringHelper.h>

#include <sprite2/RenderParams.h>
#include <sprite2/S2_RVG.h>
#include <sprite2/RenderFilter.h>
#include <glue/trans_color.h>
#include <glue/GLUE_GTxt.h>

#include <shaderlab.h>
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
		sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
		if (p.shader.filter && p.shader.filter->GetMode() == sl::FM_GRAY) {
			mgr->SetShader(sl::FILTER);
			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
			shader->SetMode(sl::FM_GRAY);
		} else {
			mgr->SetShader(sl::SPRITE2);
		}
		s2::TextboxSymbol::Draw(p, spr);
 	}
}

void Symbol::LoadResources()
{
	LoadJson(m_filepath);
}

void Symbol::DrawText(const gtxt_label_style& style, const sm::mat4& mt, const s2::Color& mul, 
					  const s2::Color& add, const std::string& text, int time, bool richtext) const
{
	std::string t_text = ee::StringHelper::ToUtf8(text);
	glue::GTxt::Instance()->Draw(style, mt, mul, add, t_text, time, richtext);
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