#include "SetValueDialog.h"

namespace d2d
{

SetValueDialog::SetValueDialog(wxWindow* parent, const wxString& title, const wxString& oldVal, const wxPoint& pos)
	: wxDialog(parent, wxID_ANY, title, pos)
{
	initLayout(oldVal);
	SetPosition(wxPoint(pos.x, pos.y - GetSize().y));
}

wxString SetValueDialog::getText() const
{
	return m_textCtrl->GetLineText(0);
}

void SetValueDialog::initLayout(const wxString& oldVal)
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);	

	m_textCtrl = new wxTextCtrl(this, wxID_ANY, oldVal);
	topSizer->Add(m_textCtrl, 0, wxALIGN_CENTRE | wxALL, 5);

	wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
	btnSizer->Add(new wxButton(this, wxID_OK), wxALL, 5);
	btnSizer->Add(new wxButton(this, wxID_CANCEL), wxALL, 5);
	topSizer->Add(btnSizer, 0, wxALL, 5);

	SetSizer(topSizer);
	topSizer->Fit(this);
}

} // d2d