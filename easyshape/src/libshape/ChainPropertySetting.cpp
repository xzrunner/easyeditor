#include "ChainPropertySetting.h"
#include "ChainShape.h"

namespace libshape
{

ChainPropertySetting::ChainPropertySetting(d2d::EditPanel* editPanel, 
										   ChainShape* chain)
	: d2d::IPropertySetting(editPanel, wxT("Chain"))
	, m_chain(chain)
{
}

void ChainPropertySetting::updatePanel(d2d::PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("Name"))->SetValue(m_chain->name);
		pg->GetProperty(wxT("X"))->SetValue(m_chain->getRect().xCenter());
		pg->GetProperty(wxT("Y"))->SetValue(m_chain->getRect().yCenter());
		pg->GetProperty(wxT("Closed"))->SetValue(m_chain->isClosed());
		pg->GetProperty(wxT("Mirror"))->SetValue(wxT("none"));
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_chain->name));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_chain->getRect().xCenter()));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_chain->getRect().yCenter()));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

		pg->Append(new wxBoolProperty(wxT("Closed"), wxPG_LABEL, m_chain->isClosed()));

		static const wxChar* mirror_labels[] = { wxT("none"),
			wxT("horizontal"), wxT("vertical"), NULL };
		pg->Append(new wxEnumProperty(wxT("Mirror"), wxPG_LABEL, mirror_labels));
	}
}

void ChainPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
	{
		m_chain->name = wxANY_AS(value, wxString);
	}
	else if (name == wxT("X"))
	{
		const float x = wxANY_AS(value, float);
		const float dx = x - m_chain->getRect().xCenter();
		std::vector<d2d::Vector> vertices = m_chain->getVertices();
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
			vertices[i].x += dx;
		m_chain->setVertices(vertices);
	}
	else if (name == wxT("Y"))
	{
		const float y = wxANY_AS(value, float);
		const float dy = y - m_chain->getRect().yCenter();
		std::vector<d2d::Vector> vertices = m_chain->getVertices();
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
			vertices[i].y += dy;
		m_chain->setVertices(vertices);
	}
	else if (name == wxT("Closed"))
	{
		m_chain->setClosed(wxANY_AS(value, bool));
	}
	else if (name == wxT("Mirror"))
	{
		int type = wxANY_AS(value, int);
		if (type == 1)
		{
			float x = m_chain->getRect().xCenter();
			std::vector<d2d::Vector> vertices = m_chain->getVertices();
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
				vertices[i].x = x * 2 - vertices[i].x;
			m_chain->setVertices(vertices);
		}
		else if (type == 2)
		{
			float y = m_chain->getRect().yCenter();
			std::vector<d2d::Vector> vertices = m_chain->getVertices();
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
				vertices[i].y = y * 2 - vertices[i].y;
			m_chain->setVertices(vertices);
		}
	}

	m_editPanel->Refresh();
}

void ChainPropertySetting::updatePropertyGrid(d2d::PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void ChainPropertySetting::enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_chain->name));

		pg->Append(new wxFloatProperty(wxT("X"), wxPG_LABEL, m_chain->getRect().xCenter()));
		pg->SetPropertyAttribute(wxT("X"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("X"), "Precision", 1);

		pg->Append(new wxFloatProperty(wxT("Y"), wxPG_LABEL, m_chain->getRect().yCenter()));
		pg->SetPropertyAttribute(wxT("Y"), wxPG_ATTR_UNITS, wxT("pixels"));
		pg->SetPropertyAttribute(wxT("Y"), "Precision", 1);

		pg->Append(new wxBoolProperty(wxT("Closed"), wxPG_LABEL, m_chain->isClosed()));

		static const wxChar* mirror_labels[] = { wxT("none"),
			wxT("horizontal"), wxT("vertical"), NULL };
		pg->Append(new wxEnumProperty(wxT("Mirror"), wxPG_LABEL, mirror_labels));
	}

	pg->GetProperty(wxT("Type"))->Enable(bEnable);
	pg->GetProperty(wxT("Name"))->Enable(bEnable);
	pg->GetProperty(wxT("X"))->Enable(bEnable);
	pg->GetProperty(wxT("Y"))->Enable(bEnable);
	pg->GetProperty(wxT("Closed"))->Enable(bEnable);
	pg->GetProperty(wxT("Mirror"))->Enable(bEnable);
}

}