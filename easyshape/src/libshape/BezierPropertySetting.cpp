#include "BezierPropertySetting.h"
#include "BezierShape.h"

namespace libshape
{

BezierPropertySetting::BezierPropertySetting(d2d::EditPanelImpl* stage, 
											 BezierShape* bezier)
	: d2d::IPropertySetting("Bezier")
	, m_stage(stage)
	, m_bezier(bezier)
{
}

void BezierPropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	bool dirty = true;
	if (name == wxT("Name"))
	{
		m_bezier->name = wxANY_AS(value, wxString);
	}
	else if (name == wxT("X"))
	{
		const float x = wxANY_AS(value, float);
		const float dx = x - m_bezier->GetRect().CenterX();
		m_bezier->Translate(d2d::Vector(dx, 0.0f));
	}
	else if (name == wxT("Y"))
	{
		const float y = wxANY_AS(value, float);
		const float dy = y - m_bezier->GetRect().CenterY();
		m_bezier->Translate(d2d::Vector(0.0f, dy));
	}
	else if (name == wxT("Mirror"))
	{
		int type = wxANY_AS(value, int);
		if (type == 1) {
			m_bezier->Mirror(true, false);
			m_bezier->createCurve();
		} else if (type == 2) {
			m_bezier->Mirror(false, true);
			m_bezier->createCurve();
		}
	}
	else
	{
		dirty = false;
	}

	if (dirty) {
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void BezierPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Name"))->SetValue(m_bezier->name);
	pg->GetProperty(wxT("X"))->SetValue(m_bezier->GetRect().CenterX());
	pg->GetProperty(wxT("Y"))->SetValue(m_bezier->GetRect().CenterY());
	pg->GetProperty(wxT("Mirror"))->SetValue(wxT("none"));
}

void BezierPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_bezier->name));

	pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_bezier->GetRect().CenterX()));
	pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_bezier->GetRect().CenterY()));
	pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

	static const wxChar* mirror_labels[] = { wxT("none"),
		wxT("horizontal"), wxT("vertical"), NULL };
	pg->Append(new wxEnumProperty(wxT("Mirror"), wxPG_LABEL, mirror_labels));
}


}