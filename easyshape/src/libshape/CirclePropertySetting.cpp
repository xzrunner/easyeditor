#include "CirclePropertySetting.h"
#include "CircleShape.h"

#include <ee/panel_msg.h>

namespace eshape
{

CirclePropertySetting::CirclePropertySetting(ee::EditPanelImpl* stage, 
											 CircleShape* circle)
	: ee::PropertySetting("Circle")
	, m_stage(stage)
	, m_circle(circle)
{
}

void CirclePropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	bool dirty = true;
	if (name == wxT("Name")) {
		m_circle->SetName(wxANY_AS(value, wxString).ToStdString());
	} else if (name == wxT("X")) {
		sm::vec2 c = m_circle->GetCenter();
		c.x = wxANY_AS(value, float);
		m_circle->SetCenter(c);
	} else if (name == wxT("Y")) {
		sm::vec2 c = m_circle->GetCenter();
		c.y = wxANY_AS(value, float);
		m_circle->SetCenter(c);
	} else if (name == "Radius") {
		float radius = wxANY_AS(value, float);
		m_circle->SetRadius(radius);
	} else {
		dirty = false;
	}

	if (dirty) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void CirclePropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Name"))->SetValue(m_circle->GetName());
	pg->GetProperty(wxT("X"))->SetValue(m_circle->GetCenter().x);
	pg->GetProperty(wxT("Y"))->SetValue(m_circle->GetCenter().y);
	pg->GetProperty(wxT("Radius"))->SetValue(m_circle->GetRadius());
}

void CirclePropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_circle->GetName()));

	pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_circle->GetCenter().x));
	pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_circle->GetCenter().y));
	pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Radius"), wxPG_LABEL, m_circle->GetRadius()));
	pg->SetPropertyAttribute(wxT("Radius"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Radius"), "Precision", 1);
}

}