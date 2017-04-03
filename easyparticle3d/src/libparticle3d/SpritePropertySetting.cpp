#include "SpritePropertySetting.h"
#include "Sprite.h"

#include <ee/EditPanelImpl.h>
#include <ee/EditPanel.h>
#include <ee/panel_msg.h>

#include <sprite2/Particle3dSprite.h>

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
		spr->SetLocal(wxANY_AS(value, bool));
	} else if (name == "Alone") {
		spr->SetAlone(wxANY_AS(value, bool));
	} else if (name == "Reuse") {
		int idx = wxANY_AS(value, int);
		s2::Particle3dSprite::ReuseType reuse = s2::Particle3dSprite::REUSE_COMMON;
		if (idx == 0) {
			reuse = s2::Particle3dSprite::REUSE_ALL;
		} else if (idx == 2) {
			reuse = s2::Particle3dSprite::REUSE_NONE;
		}
		spr->SetReuse(reuse);
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
	pg->GetProperty("Local")->SetValue(spr->IsLocal());
	pg->GetProperty("Alone")->SetValue(spr->IsAlone());

	s2::Particle3dSprite::ReuseType reuse = spr->GetReuse();
	int reuse_idx = 1;
	if (reuse == s2::Particle3dSprite::REUSE_ALL) {
		reuse_idx = 0;
	} else if (reuse == s2::Particle3dSprite::REUSE_NONE) {
		reuse_idx = 2;
	}
	pg->GetProperty("Reuse")->SetValue(reuse_idx);

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
	pg->Append(new wxBoolProperty("Local", wxPG_LABEL, spr->IsLocal()));
	pg->SetPropertyAttribute("Local", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->Append(new wxBoolProperty("Alone", wxPG_LABEL, spr->IsAlone()));
	pg->SetPropertyAttribute("Alone", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	wxArrayString choices;
	choices.push_back("All");
	choices.push_back("Common");
	choices.push_back("None");
	pg->Append(new wxEnumProperty("Reuse", wxPG_LABEL, choices, wxArrayInt(), 1));	

	pg->Append(new wxFloatProperty("Start Radius", wxPG_LABEL, spr->GetStartRadius()));
}

}