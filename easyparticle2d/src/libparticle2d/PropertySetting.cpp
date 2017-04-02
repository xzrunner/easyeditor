#include "PropertySetting.h"
#include "Sprite.h"

#include <ee/EditPanelImpl.h>
#include <ee/EditPanel.h>
#include <ee/panel_msg.h>

namespace eparticle2d
{

PropertySetting::PropertySetting(ee::EditPanelImpl* edit_impl, Sprite* spr)
	: ee::SpritePropertySetting(edit_impl, spr)
	, m_parent(edit_impl->GetEditPanel())
{
	m_type = "P2D";
}

PropertySetting::~PropertySetting()
{
}

void PropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SpritePropertySetting::OnPropertyGridChange(name, value);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	if (name == "Loop") {
		spr->SetLoop(wxANY_AS(value, bool));
	} else if (name == "Local") {
		spr->SetLocal(wxANY_AS(value, bool));
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void PropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::UpdateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(GetSprite());

	pg->GetProperty("Loop")->SetValue(spr->IsLoop());
	pg->GetProperty("Local")->SetValue(spr->IsLocal());
}

void PropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("P2D", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());

	pg->Append(new wxBoolProperty("Loop", wxPG_LABEL, spr->IsLoop()));
	pg->SetPropertyAttribute("Loop", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	pg->Append(new wxBoolProperty("Local", wxPG_LABEL, spr->IsLocal()));
	pg->SetPropertyAttribute("Local", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
}

}