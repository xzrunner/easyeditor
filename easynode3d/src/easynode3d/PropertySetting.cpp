#include "PropertySetting.h"
#include "Sprite.h"

#include <ee/StringHelper.h>

namespace enode3d
{

PropertySetting::PropertySetting(ee::EditPanelImpl* edit_impl, const std::shared_ptr<Sprite>& spr)
	: ee::SpritePropertySetting(edit_impl, spr)
{
	m_type = "Node3d";
}

void PropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SpritePropertySetting::OnPropertyGridChange(name, value);

	auto spr = std::dynamic_pointer_cast<Sprite>(GetSprite());
	if (name == "Pos3")
	{
		std::vector<std::string> keys;
		ee::StringHelper::Split(wxANY_AS(value, wxString).ToStdString(), ";", keys);
		assert(keys.size() == 3);
		sm::vec3 pos;
		ee::StringHelper::FromString(keys[0], pos.x);
		ee::StringHelper::FromString(keys[1], pos.y);
		ee::StringHelper::FromString(keys[2], pos.z);
		spr->SetPos3(pos);
	}
}

void PropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::UpdateProperties(pg);

 	auto spr = std::dynamic_pointer_cast<Sprite>(GetSprite());

	auto& pos = spr->GetPos3();
	pg->GetProperty(wxT("Pos3.X"))->SetValue(pos.x);
	pg->GetProperty(wxT("Pos3.Y"))->SetValue(pos.y);
	pg->GetProperty(wxT("Pos3.Z"))->SetValue(pos.z);
	pg->GetProperty(wxT("Pos3"))->SetValue(pg->GetProperty(wxT("Pos3"))->GenerateComposedValue());
}

void PropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("NODE3", wxPG_LABEL));

	auto spr = std::dynamic_pointer_cast<Sprite>(GetSprite());

	auto& pos = spr->GetPos3();
	wxPGProperty* pos_prop = pg->Append(new wxStringProperty(wxT("Pos3"), wxPG_LABEL, wxT("<composed>")));
	pos_prop->SetExpanded(false);
	pg->AppendIn(pos_prop, new wxFloatProperty(wxT("X"), wxPG_LABEL, pos.x));
	pg->SetPropertyAttribute(wxT("Pos3.X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Pos3.X"), "Precision", 1);
	pg->AppendIn(pos_prop, new wxFloatProperty(wxT("Y"), wxPG_LABEL, pos.y));
	pg->SetPropertyAttribute(wxT("Pos3.Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Pos3.Y"), "Precision", 1);
	pg->AppendIn(pos_prop, new wxFloatProperty(wxT("Z"), wxPG_LABEL, pos.z));
	pg->SetPropertyAttribute(wxT("Pos3.Z"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Pos3.Z"), "Precision", 1);
}

}