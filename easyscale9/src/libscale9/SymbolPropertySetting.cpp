#include "SymbolPropertySetting.h"
#include "Symbol.h"

namespace escale9
{

SymbolPropertySetting::SymbolPropertySetting(d2d::EditPanel* editPanel, Symbol* symbol)
	: d2d::IPropertySetting(editPanel, wxT("Scale9 Symbol"))
	, m_symbol(symbol)
{
}

void SymbolPropertySetting::updatePanel(d2d::PropertySettingPanel* panel)
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

void SymbolPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == "Width")
	{
		float w = wxANY_AS(value, float),
			h = m_symbol->getSize().yLength();
		m_symbol->ResizeScale9(w, h);
	}
	else if (name == "Height")
	{
		float w = m_symbol->getSize().yLength(),
			h = wxANY_AS(value, float);
		m_symbol->ResizeScale9(w, h);
	}

	m_editPanel->Refresh();
}

void SymbolPropertySetting::updatePropertyGrid(d2d::PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void SymbolPropertySetting::enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable)
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