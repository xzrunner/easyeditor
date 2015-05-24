#include "SpritePropertySetting.h"
#include "Sprite.h"

namespace eicon
{

SpritePropertySetting::SpritePropertySetting(d2d::EditPanel* editPanel, Sprite* sprite)
	: d2d::SpritePropertySetting(editPanel, sprite)
{
	m_type = wxT("Icon");
}

void SpritePropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	d2d::SpritePropertySetting::onPropertyGridChange(name, value);
	
	Sprite* sprite = static_cast<Sprite*>(GetSprite());
	if (name == wxT("Process")) {
		sprite->SetProcess(wxANY_AS(value, float));
	}
}

void SpritePropertySetting::enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable)
{
	d2d::SpritePropertySetting::enablePropertyGrid(panel, bEnable);

	wxPropertyGrid* pg = panel->getPG();
	pg->GetProperty(wxT("Process"))->Enable(bEnable);
}

void SpritePropertySetting::updateProperties(wxPropertyGrid* pg)
{
	d2d::SpritePropertySetting::updateProperties(pg);

	Sprite* sprite = static_cast<Sprite*>(GetSprite());
	pg->GetProperty(wxT("Process"))->SetValue(sprite->GetProcess());
}

void SpritePropertySetting::initProperties(wxPropertyGrid* pg)
{
	d2d::SpritePropertySetting::initProperties(pg);

	pg->Append(new wxPropertyCategory("ICON", wxPG_LABEL));

	Sprite* sprite = static_cast<Sprite*>(GetSprite());
	pg->Append(new wxFloatProperty(wxT("Process"), wxPG_LABEL, sprite->GetProcess()));
}

}