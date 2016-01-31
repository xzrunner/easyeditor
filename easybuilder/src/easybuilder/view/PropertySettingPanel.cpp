
#include "PropertySettingPanel.h"

using namespace ebuilder;

PropertySettingPanel::PropertySettingPanel(wxWindow* parent)
	: ee::PropertySettingPanel(parent)
{
	initLayout();
}

void PropertySettingPanel::initLayout()
{
	wxSizer* sizer = GetSizer();

	sizer->DeleteWindows();

	initTitle(sizer);
	initPropertyGrid(sizer);

	SetSizer(sizer);
}

void PropertySettingPanel::initTitle(wxSizer* sizer)
{
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("  Property"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 0, wxEXPAND);
}

void PropertySettingPanel::initPropertyGrid(wxSizer* sizer)
{
	m_pg = new wxPropertyGrid(
		this, 
		-1,
		wxDefaultPosition,
		wxSize(400, 800),
		wxPG_SPLITTER_AUTO_CENTER/* | wxPG_BOLD_MODIFIED*/
		);
	Connect(m_pg->GetId(), wxEVT_PG_CHANGED, wxPropertyGridEventHandler(PropertySettingPanel::onPropertyGridChange));

	sizer->Add(m_pg);

}
