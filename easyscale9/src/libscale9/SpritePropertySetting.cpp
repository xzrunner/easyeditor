#include "SpritePropertySetting.h"
#include "Sprite.h"

namespace escale9
{

SpritePropertySetting::SpritePropertySetting(d2d::EditPanelImpl* stage, Sprite* sprite)
	: d2d::SpritePropertySetting(stage, sprite)
{
	m_type = "Scale9Sprite";
}

void SpritePropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	d2d::SpritePropertySetting::OnPropertyGridChange(name, value);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
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

void SpritePropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	d2d::SpritePropertySetting::UpdateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	float w, h;
	spr->GetSize(w, h);

	pg->GetProperty(wxT("Width"))->SetValue(w);
	pg->GetProperty(wxT("Height"))->SetValue(h);
}

void SpritePropertySetting::InitProperties(wxPropertyGrid* pg)
{
	d2d::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("SCALE9", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	float w, h;
	spr->GetSize(w, h);

	pg->Append(new wxFloatProperty("Width", wxPG_LABEL, w));
	pg->Append(new wxFloatProperty("Height", wxPG_LABEL, h));
}

}