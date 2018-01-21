#include "PropertySettingPanel.h"

namespace enode3d
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	InitLayout();

	SetBackgroundColour(wxColour(229, 229, 229));
}

void PropertySettingPanel::SetPropertySetting(std::unique_ptr<PropertySetting>& setting)
{
	if (m_setting == setting) {
		return;
	}

	m_setting = std::move(setting);
	if (m_setting) {
		m_setting->UpdateFromPanel(*this);
		m_type = m_setting->GetType();
	} else {
		m_pg->Clear();
		m_type.clear();
	}
}

void PropertySettingPanel::OnPropertyGridChange(wxPropertyGridEvent& event)
{
	wxPGProperty* property = event.GetProperty();
	m_setting->OnPropertyGridChange(property->GetName().ToStdString(), property->GetValue());
}

void PropertySettingPanel::UpdatePropertyGrid()
{
	if (m_setting) {
		m_setting->UpdateFromPanel(*this);
	}
}

void PropertySettingPanel::EnablePropertyGrid(bool enable)
{
	if (m_setting) {
		m_setting->EnablePropertyGrid(*this, enable);
	}
}

void PropertySettingPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// title
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT(" Property"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 0, wxEXPAND);

	// pg
	m_pg = new wxPropertyGrid(
		this,
		-1,
		wxDefaultPosition,
		wxSize(400, 800),
		wxPG_SPLITTER_AUTO_CENTER
		| wxPG_BOLD_MODIFIED
	);
	Connect(m_pg->GetId(), wxEVT_PG_CHANGED, wxPropertyGridEventHandler(PropertySettingPanel::OnPropertyGridChange));
	sizer->Add(m_pg);

	SetSizer(sizer);
}

}