#include "SetNameDialog.h"

namespace ee
{

SetNameDialog::SetNameDialog(wxWindow* parent, const std::string& name)
	: BaseDialog(parent, "Set Name")
{
	InitLayout(name);
}

std::string SetNameDialog::GetShapeName() const
{
	return m_name->GetValue().ToStdString();
}

void SetNameDialog::InitLayout(const std::string& name)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(10);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxStaticText(this, wxID_ANY, "name"), 0, wxLEFT | wxRIGHT, 5);

		m_name = new wxTextCtrl(this, wxID_ANY, name, wxDefaultPosition, wxSize(400, -1));
		sz->Add(m_name, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(sz);
	}
	sizer->AddSpacer(10);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxButton(this, wxID_OK), 0, wxALL, 5);
		sz->Add(new wxButton(this, wxID_CANCEL), 0, wxALL, 5);
		sizer->Add(sz, 0, wxCENTER);
	}
	SetSizer(sizer);
	sizer->Fit(this);
}

}