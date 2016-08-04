#include "InputPanel.h"
#include "Sprite.h"

#include <ee/panel_msg.h>

#include <wx/wx.h>

namespace etext
{

InputPanel::InputPanel(wxWindow* parent, Sprite* spr,
					   ee::EditPanelImpl* stage_impl)
	: wxPanel(parent, wxID_ANY)
	, m_stage_impl(stage_impl)
{
	spr->Retain();
	m_spr = spr;

	InitLayout();
}

InputPanel::~InputPanel()
{
	m_spr->Release();
}

void InputPanel::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	{
		m_text_ctrl = new wxTextCtrl(this, wxID_ANY, m_spr->GetText(), 
			wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
		sizer->Add(m_text_ctrl, 12, wxEXPAND);
	}
	{
		m_enter_btn = new wxButton(this, wxID_ANY, "Enter");
		Connect(m_enter_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(InputPanel::OnEnterPress));
		sizer->Add(m_enter_btn, 1, wxEXPAND);
	}
	sizer->Layout();

	SetSizer(sizer);
}

void InputPanel::OnEnterPress(wxCommandEvent& event)
{
	std::string str = m_text_ctrl->GetValue().ToStdString();
	while (!str.empty() && str[str.size() - 1] == '\n') {
		str.erase(--str.end());
	}
	m_spr->SetText(str);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}