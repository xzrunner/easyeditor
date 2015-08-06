#include "MousePositionCMPT.h"

#include "operator/MousePositionOP.h"
#include "view/EditPanel.h"

namespace d2d
{

MousePositionCMPT::MousePositionCMPT(wxWindow* parent, const wxString& name, 
									 EditPanel* editPanel, bool create_op)
	: AbstractEditCMPT(parent, name, editPanel)
{
	if (create_op) {
		m_editOP = new MousePositionOP(editPanel, editPanel->GetStageImpl(), this);
	}
}

void MousePositionCMPT::updatePosition(const Vector& pos)
{
	m_xText->ChangeValue(wxString::FromDouble(pos.x, 1));
	m_yText->ChangeValue(wxString::FromDouble(pos.y, 1));
}

wxSizer* MousePositionCMPT::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Êó±êÎ»ÖÃ"));
	wxBoxSizer* topSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("x: ")));
		sizer->Add(m_xText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY));
		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("y: ")));
		sizer->Add(m_yText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY));
		topSizer->Add(sizer);
	}
	return topSizer;
}

} // d2d