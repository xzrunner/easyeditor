#include "ee0/WxLibraryPage.h"
#include "ee0/WxLibraryList.h"

namespace ee0
{

WxLibraryPage::WxLibraryPage(wxWindow* parent, const std::string& name, bool is_static)
	: wxWindow(parent, wxID_ANY)
	, m_list(nullptr)
	, m_name(name)
	, m_is_static(is_static)
{
	m_btn_add = m_btn_del = nullptr;

	InitLayout();
}

std::shared_ptr<WxLibraryItem> WxLibraryPage::GetItem(int idx) const
{
	return m_list->GetItem(idx);
}

void WxLibraryPage::OnAddPress(wxCommandEvent& event)
{

}

void WxLibraryPage::OnDelPress(wxCommandEvent& event)
{

}

void WxLibraryPage::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	if (!m_is_static) {
		InitButtons(sizer);
	}

	m_list = new WxLibraryList(this, "zz");
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}

void WxLibraryPage::InitButtons(wxSizer* sizer)
{
	wxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

//	InitLayoutExtend(btnSizer);

	m_btn_add = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(20, 20));
	Connect(m_btn_add->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WxLibraryPage::OnAddPress));
	btnSizer->Add(m_btn_add, 0, wxLEFT | wxRIGHT, 5);

	m_btn_del = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(20, 20));
	Connect(m_btn_del->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WxLibraryPage::OnDelPress));
	btnSizer->Add(m_btn_del, 0, wxLEFT | wxRIGHT, 5);

	//	sizer->Add(btnSizer, 0, wxALIGN_RIGHT);
	sizer->Add(btnSizer);
}

}