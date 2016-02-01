#include "SpritePropertySetting.h"
#include "Sprite.h"

namespace escale9
{

SpritePropertySetting::SpritePropertySetting(ee::EditPanelImpl* stage, Sprite* sprite)
	: ee::SpritePropertySetting(stage, sprite)
{
	m_type = "Scale9Sprite";
}

void SpritePropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SpritePropertySetting::OnPropertyGridChange(name, value);

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
	ee::SpritePropertySetting::UpdateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	float w, h;
	spr->GetSize(w, h);

	pg->GetProperty(wxT("Width"))->SetValue(w);
	pg->GetProperty(wxT("Height"))->SetValue(h);
}

void SpritePropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("SCALE9", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	float w, h;
	spr->GetSize(w, h);

	pg->Append(new wxFloatProperty("Width", wxPG_LABEL, w));
	pg->Append(new wxFloatProperty("Height", wxPG_LABEL, h));
}

}