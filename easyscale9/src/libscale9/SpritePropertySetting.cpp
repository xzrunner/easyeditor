#include "SpritePropertySetting.h"
#include "Sprite.h"

namespace escale9
{

SpritePropertySetting::SpritePropertySetting(ee::EditPanelImpl* stage, Sprite* spr)
	: ee::SpritePropertySetting(stage, spr)
{
	m_type = "Scale9Sprite";
}

void SpritePropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SpritePropertySetting::OnPropertyGridChange(name, value);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	s2::SCALE9_TYPE type = spr->GetScale9().GetType();
	if (name == wxT("Width") && type != s2::S9_3GRID_VERT) {
		sm::vec2 sz = spr->GetScale9().GetSize();
		sz.x = wxANY_AS(value, float);
		spr->Resize(sz.x, sz.y);
	} else if (name == wxT("Height") && type != s2::S9_3GRID_HORI) {
		sm::vec2 sz = spr->GetScale9().GetSize();
		sz.y = wxANY_AS(value, float);
		spr->Resize(sz.x, sz.y);
	}
}

void SpritePropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::UpdateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	sm::vec2 sz = spr->GetScale9().GetSize();
	pg->GetProperty(wxT("Width"))->SetValue(sz.x);
	pg->GetProperty(wxT("Height"))->SetValue(sz.y);
}

void SpritePropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("SCALE9", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	sm::vec2 sz = spr->GetScale9().GetSize();
	pg->Append(new wxFloatProperty("Width", wxPG_LABEL, sz.x));
	pg->Append(new wxFloatProperty("Height", wxPG_LABEL, sz.y));
}

}