#include "SetValueDialog.h"

namespace ee
{

SetValueDialog::SetValueDialog(wxWindow* parent, const std::string& title, const std::string& old_val, const wxPoint& pos)
	: wxDialog(parent, wxID_ANY, title, pos)
{
	InitLayout(old_val);
	SetPosition(wxPoint(pos.x, pos.y - GetSize().y));
}

std::string SetValueDialog::GetText() const
{
	return m_text_ctrl->GetLineText(0);
}

void SetValueDialog::InitLayout(const std::string& old_val)
{
	wxBoxSizer* top_sz = new wxBoxSizer(wxVERTICAL);	

	m_text_ctrl = new wxTextCtrl(this, wxID_ANY, old_val);
	top_sz->Add(m_text_ctrl, 0, wxALIGN_CENTRE | wxALL, 5);

	wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
	btnSizer->Add(new wxButton(this, wxID_OK), wxALL, 5);
	btnSizer->Add(new wxButton(this, wxID_CANCEL), wxALL, 5);
	top_sz->Add(btnSizer, 0, wxALL, 5);

	SetSizer(top_sz);
	top_sz->Fit(this);
}

}