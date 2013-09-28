#include "SymbolPropertySetting.h"
#include "PropertySettingPanel.h"

#include "dataset/ISymbol.h"
#include "view/EditPanel.h"

namespace d2d
{

SymbolPropertySetting::SymbolPropertySetting(EditPanel* editPanel, ISymbol* symbol)
	: IPropertySetting(editPanel, wxT("ISymbol"))
	, m_symbol(symbol)
	, m_name(NULL)
{
}

SymbolPropertySetting::SymbolPropertySetting(EditPanel* editPanel, std::string* name)
	: IPropertySetting(editPanel, wxT("ISymbol"))
	, m_symbol(NULL)
	, m_name(name)
{
}

void SymbolPropertySetting::updatePanel(PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) == m_type)
	{
		if (m_symbol)
			pg->GetProperty(wxT("Name"))->SetValue(m_symbol->name);
		else
			pg->GetProperty(wxT("Name"))->SetValue(*m_name);
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		if (m_symbol)
			pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_symbol->name));
		else
			pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, *m_name));
	}
}

void SymbolPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
	{
		if (m_symbol)
			m_symbol->name = wxANY_AS(value, wxString);
		else
			*m_name = wxANY_AS(value, wxString);
	}

	m_editPanel->Refresh();
}

void SymbolPropertySetting::updatePropertyGrid(PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void SymbolPropertySetting::enablePropertyGrid(PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		if (m_symbol)
			pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_symbol->name));
		else
			pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, *m_name));
	}

	pg->GetProperty(wxT("Type"))->Enable(bEnable);
	pg->GetProperty(wxT("Name"))->Enable(bEnable);
}
} // d2d