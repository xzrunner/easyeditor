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
	Connect(m_pg->GetId(), wxEVT_PG_CHANGED, wxPropertyGridEventHandler(PropertySettingPanel::onPropertyGridChange));

	sizer->Add(m_pg);

	SetSizer(sizer);	
}

PropertySettingPanel::~PropertySettingPanel()
{
	delete m_setting;
}

void PropertySettingPanel::setPropertySetting(IPropertySetting* setting)
{
	delete m_setting;
	m_setting = setting;

	if (m_setting)
		m_setting->updatePanel(this);
	else
		m_pg->Clear();
}

void PropertySettingPanel::onPropertyGridChange(wxPropertyGridEvent& event)
{
	wxPGProperty* property = event.GetProperty();
	m_setting->onPropertyGridChange(property->GetName(), property->GetValue());
}

void PropertySettingPanel::updatePropertyGrid()
{
	if (m_setting)
		m_setting->updatePropertyGrid(this);
}

void PropertySettingPanel::enablePropertyGrid(bool bEnable)
{
	if (m_setting)
		m_setting->enablePropertyGrid(this, bEnable);
}

} // d2d