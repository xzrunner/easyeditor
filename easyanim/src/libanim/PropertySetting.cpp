#include "PropertySetting.h"
#include "Sprite.h"
#include "Symbol.h"

namespace eanim
{

PropertySetting::PropertySetting(ee::EditPanelImpl* edit_impl, Sprite* spr)
	: ee::SpritePropertySetting(edit_impl, spr)
{
	m_type = "Animation";
}

void PropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SpritePropertySetting::OnPropertyGridChange(name, value);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	if (name == "Loop") {
		spr->SetLoop(wxANY_AS(value, bool));
	} else if (name == "Interval") {
		spr->SetInterval(wxANY_AS(value, float));
	} else if (name == "FPS") {
		spr->SetFPS(wxANY_AS(value, int));
	} else if (name == "Start") {
		int start = wxANY_AS(value, int);
		int max_time = dynamic_cast<Symbol*>(spr->GetSymbol())->GetMaxFrameIdx();
		start -= std::floor((float)start / max_time) * max_time;
		spr->SetStartTime(start);
	} else if (name == "Active") {
		spr->SetActive(wxANY_AS(value, bool));
	}
}

void PropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::UpdateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	pg->GetProperty("Loop")->SetValue(spr->IsLoop());
	pg->GetProperty("Interval")->SetValue(spr->GetInterval());
	pg->GetProperty("FPS")->SetValue(spr->GetFPS());
	pg->GetProperty("Start")->SetValue(spr->GetStartTime());
	pg->GetProperty("Active")->SetValue(spr->IsActive());
}

void PropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("ANIMATION", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());

	pg->Append(new wxBoolProperty("Loop", wxPG_LABEL, spr->IsLoop()));
	pg->SetPropertyAttribute("Loop", wxPG_BOOL_USE_CHECKBOX, spr->IsLoop(), wxPG_RECURSE);

	pg->Append(new wxFloatProperty("Interval", wxPG_LABEL, spr->GetInterval()));
	
	pg->Append(new wxIntProperty("FPS", wxPG_LABEL, spr->GetFPS()));

	pg->Append(new wxIntProperty("Start", wxPG_LABEL, spr->GetStartTime()));

	pg->Append(new wxBoolProperty("Active", wxPG_LABEL, spr->IsActive()));
	pg->SetPropertyAttribute("Active", wxPG_BOOL_USE_CHECKBOX, spr->IsLoop(), wxPG_RECURSE);
}

}