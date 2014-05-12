#include "EditNodesCMPT.h"

namespace libshape
{

EditNodesCMPT::EditNodesCMPT(wxWindow* parent, const wxString& name,
							 EditPanel* editPanel, MultiShapesImpl* shapesImpl)
	: AbstractEditCMPT(parent, name, editPanel)
{
	m_editOP = new EditNodesOP(editPanel, shapesImpl);
}

wxSizer* EditNodesCMPT::initLayout()
{
	return initEditPanel();
}

wxSizer* EditNodesCMPT::initEditPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("编辑"));
	wxBoxSizer* topSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		m_simplifySpin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, 
			wxSize(50, -1), wxSP_ARROW_KEYS, 0, 1000, 30);
		sizer->Add(m_simplifySpin);

		wxButton* btn = new wxButton(this, wxID_ANY, wxT("化简"), wxDefaultPosition, wxSize(40, -1));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EditNodesCMPT::onSimplifyTrigger));
		sizer->Add(btn);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		m_smoothSpin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, 
			wxSize(50, -1), wxSP_ARROW_KEYS, 1, 20, 10);
		sizer->Add(m_smoothSpin);

		wxButton* btn = new wxButton(this, wxID_ANY, wxT("平滑"), wxDefaultPosition, wxSize(40, -1));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EditNodesCMPT::onSmoothTrigger));
		sizer->Add(btn);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	{
		wxButton* btnOK = new wxButton(this, wxID_ANY, wxT("确定"), wxDefaultPosition, wxSize(90, -1));
		Connect(btnOK->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EditNodesCMPT::onSaveEditChanged));
		topSizer->Add(btnOK);
	}
	return topSizer;
}

void EditNodesCMPT::onSimplifyTrigger(wxCommandEvent& event)
{
	static_cast<EditNodesOP*>(m_editOP)->simplify(m_simplifySpin->GetValue() * 0.1f);
	m_editPanel->Refresh();
}

void EditNodesCMPT::onSmoothTrigger(wxCommandEvent& event)
{
	static_cast<EditNodesOP*>(m_editOP)->smooth(m_smoothSpin->GetValue());
	m_editPanel->Refresh();
}

void EditNodesCMPT::onSaveEditChanged(wxCommandEvent& event)
{
	static_cast<EditNodesOP*>(m_editOP)->updateModified();
	m_editPanel->Refresh();
}

} // d2d