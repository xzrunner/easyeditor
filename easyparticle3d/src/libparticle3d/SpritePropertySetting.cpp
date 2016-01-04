#include "SpritePropertySetting.h"
#include "Sprite.h"

namespace eparticle3d
{

SpritePropertySetting::SpritePropertySetting(d2d::EditPanelImpl* edit_impl, Sprite* sprite)
	: d2d::SpritePropertySetting(edit_impl, sprite)
	, m_parent(edit_impl->GetEditPanel())
{
	m_type = "P3D";
}

SpritePropertySetting::~SpritePropertySetting()
{
}

void SpritePropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	d2d::SpritePropertySetting::OnPropertyGridChange(name, value);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	if (name == "Loop") {
		spr->SetLoop(wxANY_AS(value, bool));
	} else if (name == "Local") {
		spr->SetLocalModeDraw(wxANY_AS(value, bool));
	}

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

void SpritePropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	d2d::SpritePropertySetting::UpdateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(GetSprite());

	pg->GetProperty("Loop")->SetValue(spr->GetLoop());
	pg->GetProperty("Local")->SetValue(spr->GetLocalModeDraw());
}

void SpritePropertySetting::InitProperties(wxPropertyGrid* pg)
{
	d2d::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("P3D", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());

	pg->Append(new wxBoolProperty("Loop", wxPG_LABEL, spr->GetLoop()));
	pg->SetPropertyAttribute("Loop", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->Append(new wxBoolProperty("Local", wxPG_LABEL, spr->GetLocalModeDraw()));
	pg->SetPropertyAttribute("Local", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
}

}