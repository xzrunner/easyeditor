#include "BezierPropertySetting.h"
#include "BezierShape.h"

#include <ee/panel_msg.h>

namespace eshape
{

BezierPropertySetting::BezierPropertySetting(ee::EditPanelImpl* stage, 
											 BezierShape* bezier)
	: ee::PropertySetting("Bezier")
	, m_stage(stage)
	, m_bezier(bezier)
{
}

void BezierPropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	bool dirty = true;
	if (name == wxT("Name"))
	{
		m_bezier->SetName(wxANY_AS(value, wxString).ToStdString());
	}
	else if (name == wxT("X"))
	{
		const float x = wxANY_AS(value, float);
		const float dx = x - m_bezier->GetBounding().Center().x;
		m_bezier->Translate(sm::vec2(dx, 0.0f));
	}
	else if (name == wxT("Y"))
	{
		const float y = wxANY_AS(value, float);
		const float dy = y - m_bezier->GetBounding().Center().y;
		m_bezier->Translate(sm::vec2(0.0f, dy));
	}
	else if (name == wxT("Mirror"))
	{
		int type = wxANY_AS(value, int);
		if (type == 1) {
			m_bezier->Mirror(true, false);
		} else if (type == 2) {
			m_bezier->Mirror(false, true);
		}
	}
	else
	{
		dirty = false;
	}

	if (dirty) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void BezierPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Name"))->SetValue(m_bezier->GetName());
	sm::vec2 center = m_bezier->GetBounding().Center();
	pg->GetProperty(wxT("X"))->SetValue(center.x);
	pg->GetProperty(wxT("Y"))->SetValue(center.y);
	pg->GetProperty(wxT("Mirror"))->SetValue(wxT("none"));
}

void BezierPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_bezier->GetName()));

	sm::vec2 center = m_bezier->GetBounding().Center();

	pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, center.x));
	pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, center.y));
	pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

	static const wxChar* mirror_labels[] = { wxT("none"),
		wxT("horizontal"), wxT("vertical"), NULL };
	pg->Append(new wxEnumProperty(wxT("Mirror"), wxPG_LABEL, mirror_labels));
}


}