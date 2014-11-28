#include "AutoRectCutCMPT.h"
#include "AutoRectCutOP.h"

namespace eimage
{

AutoRectCutCMPT::AutoRectCutCMPT(wxWindow* parent, const wxString& name, 
								 d2d::EditPanel* editpanel)
	: d2d::AbstractEditCMPT(parent, name, editpanel)
{
	m_editOP = new AutoRectCutOP(editpanel);
}

wxSizer* AutoRectCutCMPT::initLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Add Rect"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxSizer* input_sizer = new wxBoxSizer(wxVERTICAL);

		wxString choices[5] = {"8", "16", "32", "64", "128"};

		wxSizer* width_sizer = new wxBoxSizer(wxHORIZONTAL);
		width_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Width: ")));
		m_width_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 5, choices);
		m_width_choice->SetSelection(3);
		width_sizer->Add(m_width_choice);
		input_sizer->Add(width_sizer);

		wxSizer* height_sizer = new wxBoxSizer(wxHORIZONTAL);
		height_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Height: ")));
		m_height_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 5, choices);
		m_height_choice->SetSelection(3);
		height_sizer->Add(m_height_choice);
		input_sizer->Add(height_sizer);

		sizer->Add(input_sizer);
	}
	sizer->AddSpacer(5);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Add"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(AutoRectCutCMPT::OnAddRect));
		sizer->Add(btn);
	}
	top_sizer->Add(sizer);

	return top_sizer;
}

void AutoRectCutCMPT::OnAddRect(wxCommandEvent& event)
{
	int width = wxVariant(m_width_choice->GetString(m_width_choice->GetSelection())).GetInteger();
	int height = wxVariant(m_height_choice->GetString(m_height_choice->GetSelection())).GetInteger();

	AutoRectCutOP* op = static_cast<AutoRectCutOP*>(m_editOP);
	op->getRectMgr().insert(d2d::Rect(d2d::Vector(0, 0), d2d::Vector((float)width, (float)height)));

	m_editPanel->Refresh();
}

}