#include "ChainPropertySetting.h"
#include "ChainShape.h"

namespace libshape
{

ChainPropertySetting::ChainPropertySetting(ee::EditPanelImpl* stage, 
										   ChainShape* chain)
	: ee::PropertySetting("Chain")
	, m_stage(stage)
	, m_chain(chain)
{
}

void ChainPropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	bool dirty = true;
	if (name == wxT("Name"))
	{
		m_chain->name = wxANY_AS(value, wxString);
	}
	else if (name == wxT("X"))
	{
		const float x = wxANY_AS(value, float);
		const float dx = x - m_chain->GetRect().CenterX();
		m_chain->Translate(ee::Vector(dx, 0.0f));
	}
	else if (name == wxT("Y"))
	{
		const float y = wxANY_AS(value, float);
		const float dy = y - m_chain->GetRect().CenterY();
		m_chain->Translate(ee::Vector(0.0f, dy));
	}
	else if (name == wxT("Closed"))
	{
		m_chain->SetClosed(wxANY_AS(value, bool));
	}
	else if (name == wxT("Mirror"))
	{
		int type = wxANY_AS(value, int);
		if (type == 1)
		{
			float x = m_chain->GetRect().CenterX();
			std::vector<ee::Vector> vertices = m_chain->GetVertices();
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
				vertices[i].x = x * 2 - vertices[i].x;
			m_chain->Load(vertices);
		}
		else if (type == 2)
		{
			float y = m_chain->GetRect().CenterY();
			std::vector<ee::Vector> vertices = m_chain->GetVertices();
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
				vertices[i].y = y * 2 - vertices[i].y;
			m_chain->Load(vertices);
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

void ChainPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Name"))->SetValue(m_chain->name);
	pg->GetProperty(wxT("X"))->SetValue(m_chain->GetRect().CenterX());
	pg->GetProperty(wxT("Y"))->SetValue(m_chain->GetRect().CenterY());
	pg->GetProperty(wxT("Closed"))->SetValue(m_chain->IsClosed());
	pg->GetProperty(wxT("Mirror"))->SetValue(wxT("none"));
}

void ChainPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_chain->name));

	pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_chain->GetRect().CenterX()));
	pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

	pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_chain->GetRect().CenterY()));
	pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
	pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

	pg->Append(new wxBoolProperty(wxT("Closed"), wxPG_LABEL, m_chain->IsClosed()));
	pg->SetPropertyAttribute("Closed", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	static const wxChar* mirror_labels[] = { wxT("none"),
		wxT("horizontal"), wxT("vertical"), NULL };
	pg->Append(new wxEnumProperty(wxT("Mirror"), wxPG_LABEL, mirror_labels));
}

}