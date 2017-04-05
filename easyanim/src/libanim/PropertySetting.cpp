#include "PropertySetting.h"
#include "Sprite.h"
#include "Symbol.h"

#include <sprite2/UpdateParams.h>

#include <ee/panel_msg.h>

namespace libanim
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
	} else if (name == "Start Random") {
		spr->SetStartRandom(s2::UpdateParams(), wxANY_AS(value, bool));
	} else if (name == "Static") {
		spr->SetStaticTime(wxANY_AS(value, int));
		spr->SetActive(false, NULL);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	} else if (name == "Active") {
		spr->SetActive(wxANY_AS(value, bool), NULL);
	}
}

void PropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::UpdateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	pg->GetProperty("Loop")->SetValue(spr->IsLoop());
	pg->GetProperty("Interval")->SetValue(spr->GetInterval());
	pg->GetProperty("FPS")->SetValue(spr->GetFPS());
	pg->GetProperty("Start Random")->SetValue(spr->IsStartRandom());
	pg->GetProperty("Static")->SetValue(spr->GetStaticTime());
	pg->GetProperty("Active")->SetValue(spr->IsActive());
}

void PropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("ANIMATION", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());

	pg->Append(new wxBoolProperty("Loop", wxPG_LABEL, spr->IsLoop()));
	pg->SetPropertyAttribute("Loop", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxFloatProperty("Interval", wxPG_LABEL, spr->GetInterval()));
	
	pg->Append(new wxIntProperty("FPS", wxPG_LABEL, spr->GetFPS()));

	pg->Append(new wxBoolProperty("Start Random", wxPG_LABEL, spr->IsStartRandom()));
	pg->SetPropertyAttribute("Start Random", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	wxIntProperty* static_prop = new wxIntProperty("Static", wxPG_LABEL, spr->GetStaticTime());
	static_prop->SetValue(spr->GetStaticTime());
	pg->Append(static_prop);

	pg->Append(new wxBoolProperty("Active", wxPG_LABEL, spr->IsActive()));
	pg->SetPropertyAttribute("Active", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
}

}