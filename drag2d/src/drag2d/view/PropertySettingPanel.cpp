#include "PropertySettingPanel.h"
#include "IPropertySetting.h"

namespace d2d
{

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: wxWindow(parent, wxID_ANY)
	, m_setting(NULL)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	
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

PropertySettingPanel::~PropertySettingPanel()
{
	delete m_setting;
}

void PropertySettingPanel::SetPropertySetting(IPropertySetting* setting)
{
	delete m_setting;
	m_setting = setting;

	if (m_setting) {
		m_setting->UpdateFromPanel(this);
		m_type = m_setting->GetType();
	} else {
		m_pg->Clear();
		m_type = "";
	}
}

void PropertySettingPanel::OnPropertyGridChange(wxPropertyGridEvent& event)
{
	wxPGProperty* property = event.GetProperty();
	m_setting->OnPropertyGridChange(property->GetName(), property->GetValue());
}

void PropertySettingPanel::UpdatePropertyGrid()
{
	if (m_setting) {
		m_setting->UpdateFromPanel(this);
	}
}

void PropertySettingPanel::EnablePropertyGrid(bool enable)
{
	if (m_setting) {
		m_setting->EnablePropertyGrid(this, enable);
	}
}

} // d2d