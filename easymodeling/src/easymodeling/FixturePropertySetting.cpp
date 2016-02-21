#include "FixturePropertySetting.h"

#include <ee/panel_msg.h>

#include <easymodeling.h>

namespace emodeling
{

FixturePropertySetting::FixturePropertySetting(ee::EditPanelImpl* stage, Fixture* fixture)
	: ee::PropertySetting("Fixture")
	, m_stage(stage)
	, m_fixture(fixture)
{
}

void FixturePropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	bool dirty = true;
	if (name == wxT("Name"))
		m_fixture->m_name = wxANY_AS(value, wxString);
	else if (name == wxT("density"))
		m_fixture->m_density = wxANY_AS(value, float);
	else if (name == wxT("friction"))
		m_fixture->m_friction = wxANY_AS(value, float);
	else if (name == wxT("restitution"))
		m_fixture->m_restitution = wxANY_AS(value, float);
	else if (name == wxT("isSensor"))
		m_fixture->m_is_sensor= wxANY_AS(value, bool);
	else if (name == wxT("categoryBits"))
		m_fixture->m_category_bits = wxANY_AS(value, int);
	else if (name == wxT("maskBits"))
		m_fixture->m_mask_bits = wxANY_AS(value, int);
	else if (name == wxT("groupIndex"))
		m_fixture->m_group_index = wxANY_AS(value, int);
	else
		dirty = false;

	if (dirty) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void FixturePropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Name"))->SetValue(m_fixture->m_name);
	pg->GetProperty(wxT("density"))->SetValue(m_fixture->m_density);
	pg->GetProperty(wxT("friction"))->SetValue(m_fixture->m_friction);
	pg->GetProperty(wxT("restitution"))->SetValue(m_fixture->m_restitution);
	pg->GetProperty(wxT("isSensor"))->SetValue(m_fixture->m_is_sensor);
	pg->GetProperty(wxT("categoryBits"))->SetValue(m_fixture->m_category_bits);
	pg->GetProperty(wxT("maskBits"))->SetValue(m_fixture->m_mask_bits);
	pg->GetProperty(wxT("groupIndex"))->SetValue(m_fixture->m_group_index);
}

void FixturePropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_fixture->m_name));

	pg->Append(new wxFloatProperty(wxT("density"), wxPG_LABEL, m_fixture->m_density));
	pg->SetPropertyAttribute(wxT("density"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("friction"), wxPG_LABEL, m_fixture->m_friction));
	pg->SetPropertyAttribute(wxT("friction"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("restitution"), wxPG_LABEL, m_fixture->m_restitution));
	pg->SetPropertyAttribute(wxT("restitution"), "Precision", 2);

	pg->Append(new wxBoolProperty(wxT("isSensor"), wxPG_LABEL, m_fixture->m_is_sensor));
	pg->SetPropertyAttribute("isSensor", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxIntProperty(wxT("categoryBits"), wxPG_LABEL, m_fixture->m_category_bits));
	pg->SetPropertyAttribute(wxT("categoryBits"), wxPG_ATTR_MIN, 0);
	pg->SetPropertyAttribute(wxT("categoryBits"), wxPG_ATTR_MAX, 0xFFFF);

	pg->Append(new wxIntProperty(wxT("maskBits"), wxPG_LABEL, m_fixture->m_mask_bits));
	pg->SetPropertyAttribute(wxT("maskBits"), wxPG_ATTR_MIN, 0);
	pg->SetPropertyAttribute(wxT("maskBits"), wxPG_ATTR_MAX, 0xFFFF);

	pg->Append(new wxIntProperty(wxT("groupIndex"), wxPG_LABEL, m_fixture->m_group_index));
	pg->SetPropertyAttribute(wxT("groupIndex"), wxPG_ATTR_MIN, -32768);
	pg->SetPropertyAttribute(wxT("groupIndex"), wxPG_ATTR_MAX, 32767);
}

}