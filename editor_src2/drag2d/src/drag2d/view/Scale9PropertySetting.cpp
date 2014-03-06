#include "Scale9PropertySetting.h"
#include "PropertySettingPanel.h"

#include "dataset/Scale9Symbol.h"
#include "view/EditPanel.h"

namespace d2d
{
	Scale9PropertySetting::Scale9PropertySetting(EditPanel* editPanel, Scale9Symbol* symbol)
		: IPropertySetting(editPanel, wxT("Scale9 Sprite"))
		, m_symbol(symbol)
	{
	}

	void Scale9PropertySetting::updatePanel(PropertySettingPanel* panel)
	{
		wxPropertyGrid* pg = panel->getPG();

		if (getPGType(pg) == m_type)
		{
			pg->GetProperty(wxT("Width"))->SetValue(m_symbol->getSize().xLength());
			pg->GetProperty(wxT("Height"))->SetValue(m_symbol->getSize().yLength());
		}
		else
		{
			pg->Clear();

			pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

			pg->Append(new wxFloatProperty(wxT("Width"), wxPG_LABEL, m_symbol->getSize().xLength()));
			pg->SetPropertyAttribute(wxT("Width"), "Precision", 0);

			pg->Append(new wxFloatProperty(wxT("Height"), wxPG_LABEL, m_symbol->getSize().yLength()));
			pg->SetPropertyAttribute(wxT("Height"), "Precision", 0);
		}
	}

	void Scale9PropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
	{
		if (value.IsNull())
			return;

		if (name == "Width")
		{
			float w = wxANY_AS(value, float),
				h = m_symbol->getSize().yLength();
			m_symbol->resize(w, h);
		}
		else if (name == "Height")
		{
			float w = m_symbol->getSize().yLength(),
				h = wxANY_AS(value, float);
			m_symbol->resize(w, h);
		}

		m_editPanel->Refresh();
	}

	void Scale9PropertySetting::updatePropertyGrid(PropertySettingPanel* panel)
	{
		updatePanel(panel);
	}

	void Scale9PropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
	{
		wxPropertyGrid* pg = panel->getPG();

		if (getPGType(pg) != m_type)
		{
			pg->Clear();

			pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

			pg->Append(new wxFloatProperty(wxT("Width"), wxPG_LABEL, m_symbol->getSize().xLength()));
			pg->SetPropertyAttribute(wxT("Width"), "Precision", 0);

			pg->Append(new wxFloatProperty(wxT("Height"), wxPG_LABEL, m_symbol->getSize().yLength()));
			pg->SetPropertyAttribute(wxT("Height"), "Precision", 0);
		}

		pg->GetProperty(wxT("Width"))->Enable(bEnable);
		pg->GetProperty(wxT("Height"))->Enable(bEnable);
	}

}