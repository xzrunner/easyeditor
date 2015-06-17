#include "BodyPropertySetting.h"

#include <easymodeling.h>

namespace emodeling
{

BodyPropertySetting::BodyPropertySetting(d2d::EditPanel* stage, d2d::ISprite* sprite)
	: d2d::IPropertySetting("Body")
	, m_stage(stage)
{
	m_body = static_cast<libmodeling::Body*>(sprite->getUserData());
	assert(m_body);
}

void BodyPropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
		m_body->name = wxANY_AS(value, wxString);
	else if (name == wxT("Type"))
		m_body->type = libmodeling::Body::Type(wxANY_AS(value, int));
	else if (name == wxT("LinearDamping"))
		m_body->linearDamping = wxANY_AS(value, float);
	else if (name == wxT("AngularDamping"))
		m_body->angularDamping = wxANY_AS(value, float);
	else if (name == wxT("AllowSleep"))
		m_body->allowSleep = wxANY_AS(value, bool);
	else if (name == wxT("Bullet"))
		m_body->bullet = wxANY_AS(value, bool);
	else if (name == wxT("Active"))
		m_body->active = wxANY_AS(value, bool);
	else if (name == wxT("GravityScale"))
		m_body->gravityScale = wxANY_AS(value, float);

	m_stage->Refresh();
}

void BodyPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Name"))->SetValue(m_body->name);

	pg->GetProperty(wxT("Type"))->SetValue((int)m_body->type);

	pg->GetProperty(wxT("LinearDamping"))->SetValue(m_body->linearDamping);

	pg->GetProperty(wxT("AngularDamping"))->SetValue(m_body->angularDamping);

	pg->GetProperty(wxT("AllowSleep"))->SetValue(m_body->allowSleep);

	pg->GetProperty(wxT("Bullet"))->SetValue(m_body->bullet);

	pg->GetProperty(wxT("Active"))->SetValue(m_body->active);

	pg->GetProperty(wxT("GravityScale"))->SetValue(m_body->gravityScale);
}

void BodyPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	//		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_body->name));

	wxPGChoices eech;
	eech.Add(wxT("static"));
	eech.Add(wxT("kinematic"));
	eech.Add(wxT("dynamic"));
	wxEnumProperty* typeChoice = new wxEnumProperty(wxT("Type"), wxPG_LABEL, eech);
	typeChoice->SetChoiceSelection((int)m_body->type);
	pg->Append(typeChoice);

	pg->Append(new wxFloatProperty(wxT("LinearDamping"), wxPG_LABEL, m_body->linearDamping));

	pg->Append(new wxFloatProperty(wxT("AngularDamping"), wxPG_LABEL, m_body->angularDamping));

	pg->Append(new wxBoolProperty(wxT("AllowSleep"), wxPG_LABEL, m_body->allowSleep));
	pg->SetPropertyAttribute("AllowSleep", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxBoolProperty(wxT("Bullet"), wxPG_LABEL, m_body->bullet));
	pg->SetPropertyAttribute("Bullet", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxBoolProperty(wxT("Active"), wxPG_LABEL, m_body->active));
	pg->SetPropertyAttribute("Active", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxFloatProperty(wxT("GravityScale"), wxPG_LABEL, m_body->gravityScale));
}

}