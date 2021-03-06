#include "ToolbarPanel.h"
#include "Symbol.h"

#include <wx/spinctrl.h>

#include <ee/panel_msg.h>

namespace escale9
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, ee::EditPanel* stage, const std::shared_ptr<Symbol>& sym)
	: wxPanel(parent)
	, m_stage(stage)
	, m_sym(sym)
{
	InitLayout();
}

void ToolbarPanel::InitLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("size"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	wxBoxSizer* wSizer = new wxBoxSizer(wxHORIZONTAL);
	wSizer->Add(new wxStaticText(this, wxID_ANY, wxT("width: ")));
	m_wSpin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, 
		wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, 10, 10000, m_sym->GetBounding().Size().x);
	Connect(m_wSpin->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onChangeSize));
	wSizer->Add(m_wSpin);
	sizer->Add(wSizer);

	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
	hSizer->Add(new wxStaticText(this, wxID_ANY, wxT("height: ")));
	m_hSpin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, 
		wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, 10, 10000, m_sym->GetBounding().Size().y);
	Connect(m_hSpin->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onChangeSize));
	hSizer->Add(m_hSpin);
	sizer->Add(hSizer);

	SetSizer(sizer);
}

void ToolbarPanel::onChangeSize(wxSpinEvent& event)
{
	ee::SetWndDirtySJ::Instance()->SetDirty();
	const float w = m_wSpin->GetValue(),
		h = m_hSpin->GetValue();
	m_sym->Resize(w, h);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}