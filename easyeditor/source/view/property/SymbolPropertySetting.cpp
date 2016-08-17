#include "SymbolPropertySetting.h"
#include "PropertySettingPanel.h"
#include "Symbol.h"
#include "EditPanelImpl.h"
#include "StageCanvas.h"
#include "panel_msg.h"
#include "FileHelper.h"

namespace ee
{

SymbolPropertySetting::SymbolPropertySetting(Symbol* sym)
	: PropertySetting("Symbol")
	, m_sym(sym)
	, m_name(NULL)
{
}

SymbolPropertySetting::SymbolPropertySetting(std::string* name,
											 std::string* tag)
	: PropertySetting("Symbol")
	, m_sym(NULL)
	, m_name(name)
	, m_tag(tag)
{
}

void SymbolPropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	bool dirty = true;
	if (name == wxT("Name"))
	{
		if (m_sym) {
			std::string name = wxANY_AS(value, wxString);
			m_sym->name = name;
			m_sym->SetName(name);
			RefreshPanelSJ::Instance()->Refresh();
		} else if (m_name) {
			*m_name = wxANY_AS(value, wxString);
		}
	}
	else if (name == "Tag") 
	{
		if (m_sym) {
			m_sym->tag = wxANY_AS(value, wxString);
		} else if (m_tag) {
			*m_tag = wxANY_AS(value, wxString);
		}
	}
	else
	{
		dirty = false;
	}

	if (dirty) {
		SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void SymbolPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	if (m_sym) {
		pg->GetProperty(wxT("Filepath"))->SetValue(m_sym->GetFilepath());
		pg->GetProperty(wxT("Name"))->SetValue(m_sym->name);
		pg->GetProperty(wxT("Tag"))->SetValue(m_sym->tag);
	} else {
		if (m_name) {
			pg->GetProperty(wxT("Name"))->SetValue(*m_name);
		}
		if (m_tag) {
			pg->GetProperty(wxT("Tag"))->SetValue(*m_tag);
		}
	}
}

void SymbolPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	if (m_sym) {
		pg->Append(new wxStringProperty(wxT("Filepath"), wxPG_LABEL, m_sym->GetFilepath()));
		pg->SetPropertyReadOnly("Filepath");

		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_sym->name));
		pg->Append(new wxStringProperty(wxT("Tag"), wxPG_LABEL, m_sym->tag));
	} else {
		if (m_name) {
			pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, *m_name));
		}
		if (m_tag) {
			pg->Append(new wxStringProperty(wxT("Tag"), wxPG_LABEL, *m_tag));
		}
	}
}

}