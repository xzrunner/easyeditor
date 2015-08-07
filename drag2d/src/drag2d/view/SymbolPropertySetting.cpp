#include "SymbolPropertySetting.h"
#include "PropertySettingPanel.h"

#include "dataset/ISymbol.h"
#include "view/EditPanelImpl.h"
#include "view/IStageCanvas.h"

namespace d2d
{

SymbolPropertySetting::SymbolPropertySetting(EditPanelImpl* stage, ISymbol* symbol)
	: IPropertySetting("Symbol")
	, m_stage(stage)
	, m_symbol(symbol)
	, m_name(NULL)
{
}

SymbolPropertySetting::SymbolPropertySetting(EditPanelImpl* stage, std::string* name)
	: IPropertySetting("Symbol")
	, m_stage(stage)
	, m_symbol(NULL)
	, m_name(name)
{
}

void SymbolPropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	bool dirty = true;
	if (name == wxT("Name"))
	{
		if (m_symbol)
			m_symbol->name = wxANY_AS(value, wxString);
		else
			*m_name = wxANY_AS(value, wxString);
	}
	else
	{
		dirty = false;
	}

	if (dirty) {
		m_stage->SetCanvasDirty();
	}
}

void SymbolPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	if (m_symbol)
		pg->GetProperty(wxT("Name"))->SetValue(m_symbol->name);
	else
		pg->GetProperty(wxT("Name"))->SetValue(*m_name);
}

void SymbolPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	if (m_symbol)
		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_symbol->name));
	else
		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, *m_name));
}

} // d2d