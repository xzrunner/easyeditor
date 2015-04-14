#include "SpritePropertySetting.h"
#include "Sprite.h"

namespace escale9
{

SpritePropertySetting::SpritePropertySetting(d2d::EditPanel* editPanel, Sprite* sprite)
	: d2d::SpritePropertySetting(editPanel, sprite)
{
	m_type = wxT("Scale9");
}

void SpritePropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	d2d::SpritePropertySetting::onPropertyGridChange(name, value);

	Sprite* spr = static_cast<Sprite*>(m_sprite);
	Scale9Type type = spr->GetScale9Type();
	if (name == wxT("Width") && type != e_3GridVer) {
		float w, h;
		spr->GetSize(w, h);
		w = wxANY_AS(value, float);
		spr->SetSize(w, h);
	} else if (name == wxT("Height") && type != e_3GridHor) {
		float w, h;
		spr->GetSize(w, h);
		h = wxANY_AS(value, float);
		spr->SetSize(w, h);		
	}
}

void SpritePropertySetting::enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable)
{
	d2d::SpritePropertySetting::enablePropertyGrid(panel, bEnable);

	wxPropertyGrid* pg = panel->getPG();
	pg->GetProperty(wxT("Width"))->Enable(bEnable);
	pg->GetProperty(wxT("Height"))->Enable(bEnable);
}

void SpritePropertySetting::updateProperties(wxPropertyGrid* pg)
{
	d2d::SpritePropertySetting::updateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(m_sprite);
	float w, h;
	spr->GetSize(w, h);

	pg->GetProperty(wxT("Width"))->SetValue(w);
	pg->GetProperty(wxT("Height"))->SetValue(h);
}

void SpritePropertySetting::initProperties(wxPropertyGrid* pg)
{
	d2d::SpritePropertySetting::initProperties(pg);

	pg->Append(new wxPropertyCategory("SCALE9", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(m_sprite);
	float w, h;
	spr->GetSize(w, h);

	pg->Append(new wxFloatProperty("Width", wxPG_LABEL, w));
	pg->Append(new wxFloatProperty("Height", wxPG_LABEL, h));
}

}