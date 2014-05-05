#include "CirclePropertySetting.h"
#include "CircleShape.h"

namespace libshape
{

CirclePropertySetting::CirclePropertySetting(d2d::EditPanel* editPanel, 
											 CircleShape* circle)
	: d2d::IPropertySetting(editPanel, wxT("Circle"))
	, m_circle(circle)
{
}

void CirclePropertySetting::updatePanel(d2d::PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Name"))->SetValue(m_circle->name);
		pg->GetProperty(wxT("X"))->SetValue(m_circle->center.x);
		pg->GetProperty(wxT("Y"))->SetValue(m_circle->center.y);
		pg->GetProperty(wxT("Radius"))->SetValue(m_circle->radius);
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_circle->name));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_circle->center.x));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_circle->center.y));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Radius"), wxPG_LABEL, m_circle->radius));
		pg->SetPropertyAttribute(wxT("Radius"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Radius"), "Precision", 1);
	}
}

void CirclePropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
		m_circle->name = wxANY_AS(value, wxString);
	if (name == wxT("X"))
		m_circle->center.x = wxANY_AS(value, float);
	else if (name == wxT("Y"))
		m_circle->center.y = wxANY_AS(value, float);
	else if (name == "Radius")
		m_circle->radius = wxANY_AS(value, float);

	m_editPanel->Refresh();
}

void CirclePropertySetting::updatePropertyGrid(d2d::PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void CirclePropertySetting::enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_circle->name));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_circle->center.x));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_circle->center.y));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Radius"), wxPG_LABEL, m_circle->radius));
		pg->SetPropertyAttribute(wxT("Radius"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Radius"), "Precision", 1);
	}

	pg->GetProperty(wxT("Type"))->Enable(bEnable);
	pg->GetProperty(wxT("Name"))->Enable(bEnable);
	pg->GetProperty(wxT("X"))->Enable(bEnable);
	pg->GetProperty(wxT("Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Radius"))->Enable(bEnable);
}

}