#include "SymbolPropertySetting.h"
#include "Symbol.h"

namespace escale9
{

SymbolPropertySetting::SymbolPropertySetting(d2d::EditPanel* editPanel, Symbol* symbol)
	: d2d::SymbolPropertySetting(editPanel, symbol)
	, m_symbol(symbol)
{
	m_type = "Scale9Symbol";
}

void SymbolPropertySetting::OnPropertyGridChange(const wxString& name, const wxAny& value)
{
	d2d::SymbolPropertySetting::OnPropertyGridChange(name, value);

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

	m_stage->Refresh();
}

void SymbolPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	d2d::SymbolPropertySetting::UpdateProperties(pg);

	pg->GetProperty(wxT("Width"))->SetValue(m_symbol->getSize().xLength());
	pg->GetProperty(wxT("Height"))->SetValue(m_symbol->getSize().yLength());
}

void SymbolPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	d2d::SymbolPropertySetting::InitProperties(pg);

	pg->Clear();

	pg->Append(new wxFloatProperty(wxT("Width"), wxPG_LABEL, m_symbol->getSize().xLength()));
	pg->SetPropertyAttribute(wxT("Width"), "Precision", 0);

	pg->Append(new wxFloatProperty(wxT("Height"), wxPG_LABEL, m_symbol->getSize().yLength()));
	pg->SetPropertyAttribute(wxT("Height"), "Precision", 0);
}

}