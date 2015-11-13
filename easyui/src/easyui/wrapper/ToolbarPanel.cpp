#include "ToolBarPanel.h"

namespace eui
{
namespace wrapper
{

ToolbarPanel::ToolbarPanel(wxWindow* parent)
	: d2d::ToolbarPanel(parent, NULL)
{
	SetSizer(initLayout());	
}

std::string ToolbarPanel::GetType() const
{
	return m_type_text->GetValue().ToStdString();
}

void ToolbarPanel::SetType(const std::string& type)
{
	m_type_text->SetValue(type);
}

std::string ToolbarPanel::GetTag() const
{
	return m_tag_text->GetValue().ToStdString();
}

void ToolbarPanel::SetTag(const std::string& tag)
{
	m_tag_text->SetValue(tag);
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "Type"), 0, wxLEFT | wxRIGHT, 5);

		m_type_text = new wxTextCtrl(this, wxID_ANY);
		csizer->Add(m_type_text);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(20);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "Tag"), 0, wxLEFT | wxRIGHT, 5);

		m_tag_text = new wxTextCtrl(this, wxID_ANY);
		csizer->Add(m_tag_text);

		sizer->Add(csizer);
	}
	return sizer;
}

}
}