#include "SpritePropertySetting.h"
#include "Sprite.h"

namespace eicon
{

SpritePropertySetting::SpritePropertySetting(ee::EditPanelImpl* stage, Sprite* spr)
	: ee::SpritePropertySetting(stage, spr)
{
	m_type = "IconSprite";
}

void SpritePropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SpritePropertySetting::OnPropertyGridChange(name, value);
	
	Sprite* spr = static_cast<Sprite*>(GetSprite());
	if (name == wxT("Process")) {
		spr->SetProcess(wxANY_AS(value, float));
	}
}

void SpritePropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::UpdateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	pg->GetProperty(wxT("Process"))->SetValue(spr->GetProcess());
}

void SpritePropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("ICON", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	pg->Append(new wxFloatProperty(wxT("Process"), wxPG_LABEL, spr->GetProcess()));
}

}