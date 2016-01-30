#include "PolygonPropertySetting.h"
#include "PolygonShape.h"

namespace libshape
{

PolygonPropertySetting::PolygonPropertySetting(d2d::EditPanelImpl* stage, 
											   PolygonShape* poly)
	: IPropertySetting("Polygon")
	, m_stage(stage)
{
	poly->Retain();
	m_poly = poly;
}

PolygonPropertySetting::~PolygonPropertySetting()
{
	m_poly->Release();	
}

void PolygonPropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	bool dirty = true;
	if (name == wxT("Name"))
	{
		m_poly->name = wxANY_AS(value, wxString);
	}
	else if (name == wxT("X"))
	{
		const float x = wxANY_AS(value, float);
		const float dx = x - m_poly->GetRect().CenterX();
		m_poly->Translate(d2d::Vector(dx, 0.0f));
		m_poly->refresh();
	}
	else if (name == wxT("Y"))
	{
		const float y = wxANY_AS(value, float);
		const float dy = y - m_poly->GetRect().CenterY();
		m_poly->Translate(d2d::Vector(0.0f, dy));
		m_poly->refresh();
	}
	else if (name == wxT("Mirror"))
	{
		int type = wxANY_AS(value, int);
		if (type == 1)
		{
			float x = m_poly->GetRect().CenterX();
			std::vector<d2d::Vector> vertices = m_poly->GetVertices();
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
				vertices[i].x = x * 2 - vertices[i].x;
			m_poly->Load(vertices);
			m_poly->refresh();
		}
		else if (type == 2)
		{
			float y = m_poly->GetRect().CenterY();
			std::vector<d2d::Vector> vertices = m_poly->GetVertices();
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
				vertices[i].y = y * 2 - vertices[i].y;
			m_poly->Load(vertices);
			m_poly->refresh();
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

void PolygonPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Name"))->SetValue(m_poly->name);
	pg->GetProperty(wxT("X"))->SetValue(m_poly->GetRect().CenterX());
	pg->GetProperty(wxT("Y"))->SetValue(m_poly->GetRect().CenterY());
	pg->GetProperty(wxT("Mirror"))->SetValue(wxT("none"));
}

void PolygonPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_poly->name));

	pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_poly->GetRect().CenterX()));
	pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_poly->GetRect().CenterY()));
	pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

	static const wxChar* mirror_labels[] = { wxT("none"),
		wxT("horizontal"), wxT("vertical"), NULL };
	pg->Append(new wxEnumProperty(wxT("Mirror"), wxPG_LABEL, mirror_labels));
}

}