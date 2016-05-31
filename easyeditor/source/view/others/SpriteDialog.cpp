#include "SpriteDialog.h"
#include "Sprite.h"

namespace ee
{

SpriteDialog::SpriteDialog(wxWindow* parent, Sprite* spr)
	: BaseDialog(parent, "Sprite")
	, m_spr(spr)
{
	InitLayout();
}

void SpriteDialog::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(10);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxStaticText(this, wxID_ANY, "name"), 0, wxLEFT | wxRIGHT, 5);

		m_name_ctrl = new wxTextCtrl(this, wxID_ANY, m_spr->GetName(), wxDefaultPosition, wxSize(400, -1));
		sz->Add(m_name_ctrl, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(sz);
	}
	sizer->AddSpacer(10);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxStaticText(this, wxID_ANY, "tag"), 0, wxLEFT | wxRIGHT, 5);

		m_tag_ctrl = new wxTextCtrl(this, wxID_ANY, m_spr->GetTag(), wxDefaultPosition, wxSize(400, -1));
		sz->Add(m_tag_ctrl, 0, wxLEFT | wxRIGHT, 5);

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