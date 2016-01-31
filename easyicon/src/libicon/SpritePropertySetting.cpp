#include "SpritePropertySetting.h"
#include "Sprite.h"

namespace eicon
{

SpritePropertySetting::SpritePropertySetting(ee::EditPanelImpl* stage, Sprite* sprite)
	: ee::SpritePropertySetting(stage, sprite)
{
	m_type = "IconSprite";
}

void SpritePropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	ee::SpritePropertySetting::OnPropertyGridChange(name, value);
	
	Sprite* sprite = static_cast<Sprite*>(GetSprite());
	if (name == wxT("Process")) {
		sprite->SetProcess(wxANY_AS(value, float));
	}
}

void SpritePropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::UpdateProperties(pg);

	Sprite* sprite = static_cast<Sprite*>(GetSprite());
	pg->GetProperty(wxT("Process"))->SetValue(sprite->GetProcess());
}

void SpritePropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("ICON", wxPG_LABEL));

	Sprite* sprite = static_cast<Sprite*>(GetSprite());
	pg->Append(new wxFloatProperty(wxT("Process"), wxPG_LABEL, sprite->GetProcess()));
}

}