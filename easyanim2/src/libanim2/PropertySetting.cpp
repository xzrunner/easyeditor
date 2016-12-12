#include "PropertySetting.h"
#include "Sprite.h"
#include "Symbol.h"

#include <ee/panel_msg.h>

namespace libanim2
{

PropertySetting::PropertySetting(ee::EditPanelImpl* edit_impl, Sprite* spr)
	: ee::SpritePropertySetting(edit_impl, spr)
{
	m_type = "Anim2";
}

void PropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SpritePropertySetting::OnPropertyGridChange(name, value);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	if (name == "Static") {
		spr->SetStaticTime(wxANY_AS(value, int));
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void PropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::UpdateProperties(pg);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	pg->GetProperty("Static")->SetValue(spr->GetStaticTime());
}

void PropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("ANIM2", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());

	wxIntProperty* static_prop = new wxIntProperty("Static", wxPG_LABEL, spr->GetStaticTime());
	static_prop->SetValue(spr->GetStaticTime());
	pg->Append(static_prop);
}

}