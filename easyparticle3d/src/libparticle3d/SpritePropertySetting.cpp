#include "SpritePropertySetting.h"
#include "Sprite.h"

#include <ee/EditPanelImpl.h>
#include <ee/EditPanel.h>
#include <ee/panel_msg.h>

namespace eparticle3d
{

SpritePropertySetting::SpritePropertySetting(ee::EditPanelImpl* edit_impl, Sprite* spr)
	: ee::SpritePropertySetting(edit_impl, spr)
	, m_parent(edit_impl->GetEditPanel())
{
	m_type = "P3D";
}

SpritePropertySetting::~SpritePropertySetting()
{
}

void SpritePropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SpritePropertySetting::OnPropertyGridChange(name, value);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	if (name == "Loop") {
		spr->SetLoop(wxANY_AS(value, bool));
	} else if (name == "Local") {
		bool local = wxANY_AS(value, bool);
		spr->SetLocalModeDraw(local);
		if (!local) {
			m_pg->GetProperty("Reuse")->SetValue(false);
			spr->SetReuse(false);
		}
	} else if (name == "Alone") {
		spr->SetAlone(wxANY_AS(value, bool));
	} else if (name == "Reuse") {
		bool reuse = wxANY_AS(value, bool);
		spr->SetReuse(reuse);
		if (reuse) {
			m_pg->GetProperty("Local")->SetValue(true);
			spr->SetLocalModeDraw(true);
		}
	} else if (name == "Start Radius") {
		float radius = wxANY_AS(value, float);
		spr->SetStartRadius(radius);
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void SpritePropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	m_pg = pg;

	ee::SpritePropertySetting::UpdateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(GetSprite());

	pg->GetProperty("Loop")->SetValue(spr->IsLoop());
	pg->GetProperty("Local")->SetValue(spr->IsLocalModeDraw());
	pg->GetProperty("Alone")->SetValue(spr->IsAlone());
	pg->GetProperty("Reuse")->SetValue(spr->IsReuse());

	pg->GetProperty("Start Radius")->SetValue(spr->GetStartRadius());
}

void SpritePropertySetting::InitProperties(wxPropertyGrid* pg)
{
	m_pg = pg;

	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("P3D SPR", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());

	pg->Append(new wxBoolProperty("Loop", wxPG_LABEL, spr->IsLoop()));
	pg->SetPropertyAttribute("Loop", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->Append(new wxBoolProperty("Local", wxPG_LABEL, spr->IsLocalModeDraw()));
	pg->SetPropertyAttribute("Local", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->Append(new wxBoolProperty("Alone", wxPG_LABEL, spr->IsAlone()));
	pg->SetPropertyAttribute("Alone", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->Append(new wxBoolProperty("Reuse", wxPG_LABEL, spr->IsReuse()));
	pg->SetPropertyAttribute("Reuse", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxPropertyCategory("P3D SPR", wxPG_LABEL));

	pg->Append(new wxFloatProperty("Start Radius", wxPG_LABEL, spr->GetStartRadius()));
}

}