#include "SpritePropertySetting.h"
#include "Sprite.h"

namespace eicon
{

SpritePropertySetting::SpritePropertySetting(d2d::EditPanel* editPanel, Sprite* sprite)
	: d2d::SpritePropertySetting(editPanel, sprite)
{
	m_type = "IconSprite";
}

void SpritePropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	d2d::SpritePropertySetting::OnPropertyGridChange(name, value);
	
	Sprite* sprite = static_cast<Sprite*>(GetSprite());
	if (name == wxT("Process")) {
		sprite->SetProcess(wxANY_AS(value, float));
	}
}

void SpritePropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	d2d::SpritePropertySetting::UpdateProperties(pg);

	Sprite* sprite = static_cast<Sprite*>(GetSprite());
	pg->GetProperty(wxT("Process"))->SetValue(sprite->GetProcess());
}

void SpritePropertySetting::InitProperties(wxPropertyGrid* pg)
{
	d2d::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("ICON", wxPG_LABEL));

	Sprite* sprite = static_cast<Sprite*>(GetSprite());
	pg->Append(new wxFloatProperty(wxT("Process"), wxPG_LABEL, sprite->GetProcess()));
}

}