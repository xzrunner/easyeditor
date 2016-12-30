#include "SprPropSetting.h"
#include "Sprite.h"

#include <ee/EditPanelImpl.h>
#include <ee/EditPanel.h>
#include <ee/panel_msg.h>

namespace etrail
{

SprPropSetting::SprPropSetting(ee::EditPanelImpl* edit_impl, Sprite* spr)
	: ee::SpritePropertySetting(edit_impl, spr)
	, m_parent(edit_impl->GetEditPanel())
{
	m_type = "TRAIL";
}

SprPropSetting::~SprPropSetting()
{
}

void SprPropSetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SpritePropertySetting::OnPropertyGridChange(name, value);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	if (name == "Local") {
		bool local = wxANY_AS(value, bool);
		spr->SetLocal(local);
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void SprPropSetting::UpdateProperties(wxPropertyGrid* pg)
{
	m_pg = pg;

	ee::SpritePropertySetting::UpdateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(GetSprite());

	pg->GetProperty("Local")->SetValue(spr->IsLocal());
}

void SprPropSetting::InitProperties(wxPropertyGrid* pg)
{
	m_pg = pg;

	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("TRAIL SPR", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());

	pg->Append(new wxBoolProperty("Local", wxPG_LABEL, spr->IsLocal()));
	pg->SetPropertyAttribute("Local", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
}

}