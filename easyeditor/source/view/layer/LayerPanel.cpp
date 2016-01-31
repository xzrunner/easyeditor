#include "LayerPanel.h"
#include "LayerList.h"

namespace ee
{

LayerPanel::LayerPanel(wxWindow* parent)
	: wxPanel(parent)
{
	m_list = new LayerList(this);
	InitLayout();
}

void LayerPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(InitHeaderLayout(), 0, wxEXPAND);
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}

wxSizer* LayerPanel::InitHeaderLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	// title
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT(" Layer"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 1, wxALIGN_LEFT);

	// add
	wxButton* btn_add = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(20, 20));
	Connect(btn_add->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayerPanel::OnAddPress));
	sizer->Add(btn_add, 0, wxLEFT | wxRIGHT, 5);

	// del
	wxButton* btn_del = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(20, 20));
	Connect(btn_del->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayerPanel::OnDelPress));
	sizer->Add(btn_del, 0, wxLEFT | wxRIGHT, 5);

	sizer->Fit(this);

	return sizer;
}

void LayerPanel::OnAddPress(wxCommandEvent& event)
{
	m_list->Insert();
}

void LayerPanel::OnDelPress(wxCommandEvent& event)
{
	m_list->Remove();
}

}