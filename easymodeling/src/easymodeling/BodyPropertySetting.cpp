#include "BodyPropertySetting.h"

#include <ee/Sprite.h>
#include <ee/panel_msg.h>

#include <easymodeling.h>

namespace emodeling
{

BodyPropertySetting::BodyPropertySetting(ee::EditPanelImpl* stage, ee::Sprite* sprite)
	: ee::PropertySetting("Body")
	, m_stage(stage)
{
	m_body = static_cast<Body*>(sprite->GetUserData());
	assert(m_body);
}

void BodyPropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	bool dirty = true;
	if (name == wxT("Name"))
		m_body->m_name = wxANY_AS(value, wxString);
	else if (name == wxT("Type"))
		m_body->m_type = Body::Type(wxANY_AS(value, int));
	else if (name == wxT("LinearDamping"))
		m_body->m_linear_damping = wxANY_AS(value, float);
	else if (name == wxT("AngularDamping"))
		m_body->m_angular_damping = wxANY_AS(value, float);
	else if (name == wxT("AllowSleep"))
		m_body->m_allow_sleep = wxANY_AS(value, bool);
	else if (name == wxT("Bullet"))
		m_body->m_bullet = wxANY_AS(value, bool);
	else if (name == wxT("Active"))
		m_body->m_active = wxANY_AS(value, bool);
	else if (name == wxT("GravityScale"))
		m_body->m_gravity_scale = wxANY_AS(value, float);
	else
		dirty = false;

	if (dirty) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void BodyPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Name"))->SetValue(m_body->m_name);

	pg->GetProperty(wxT("Type"))->SetValue((int)m_body->m_type);

	pg->GetProperty(wxT("LinearDamping"))->SetValue(m_body->m_linear_damping);

	pg->GetProperty(wxT("AngularDamping"))->SetValue(m_body->m_angular_damping);

	pg->GetProperty(wxT("AllowSleep"))->SetValue(m_body->m_allow_sleep);

	pg->GetProperty(wxT("Bullet"))->SetValue(m_body->m_bullet);

	pg->GetProperty(wxT("Active"))->SetValue(m_body->m_active);

	pg->GetProperty(wxT("GravityScale"))->SetValue(m_body->m_gravity_scale);
}

void BodyPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	//		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_body->m_name));

	wxPGChoices eech;
	eech.Add(wxT("static"));
	eech.Add(wxT("kinematic"));
	eech.Add(wxT("dynamic"));
	wxEnumProperty* typeChoice = new wxEnumProperty(wxT("Type"), wxPG_LABEL, eech);
	typeChoice->SetChoiceSelection((int)m_body->m_type);
	pg->Append(typeChoice);

	pg->Append(new wxFloatProperty(wxT("LinearDamping"), wxPG_LABEL, m_body->m_linear_damping));

	pg->Append(new wxFloatProperty(wxT("AngularDamping"), wxPG_LABEL, m_body->m_angular_damping));

	pg->Append(new wxBoolProperty(wxT("AllowSleep"), wxPG_LABEL, m_body->m_allow_sleep));
	pg->SetPropertyAttribute("AllowSleep", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxBoolProperty(wxT("Bullet"), wxPG_LABEL, m_body->m_bullet));
	pg->SetPropertyAttribute("Bullet", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxBoolProperty(wxT("Active"), wxPG_LABEL, m_body->m_active));
	pg->SetPropertyAttribute("Active", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxFloatProperty(wxT("GravityScale"), wxPG_LABEL, m_body->m_gravity_scale));
}

}