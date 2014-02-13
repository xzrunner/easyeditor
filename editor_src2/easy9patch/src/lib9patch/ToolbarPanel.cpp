#include "ToolbarPanel.h"

#include <wx/spinctrl.h>

namespace lib9patch
{
ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::Patch9Symbol* symbol)
	: wxPanel(parent)
	, m_symbol(symbol)
{
	initLayout();
}

void ToolbarPanel::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("size"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	wxBoxSizer* wSizer = new wxBoxSizer(wxHORIZONTAL);
	wSizer->Add(new wxStaticText(this, wxID_ANY, wxT("width: ")));
	m_wSpin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, 
		wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, 10, 10000, m_symbol->getSize().xLength());
	Connect(m_wSpin->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onChangeSize));
	wSizer->Add(m_wSpin);
	sizer->Add(wSizer);

	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
	hSizer->Add(new wxStaticText(this, wxID_ANY, wxT("height: ")));
	m_hSpin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, 
		wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, 10, 10000, m_symbol->getSize().yLength());
	Connect(m_hSpin->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onChangeSize));
	hSizer->Add(m_hSpin);
	sizer->Add(hSizer);

	SetSizer(sizer);
}

void ToolbarPanel::onChangeSize(wxSpinEvent& event)
{
	const float w = m_wSpin->GetValue(),
		h = m_hSpin->GetValue();
	m_symbol->resize(w, h);
	m_parent->Refresh();
}

}