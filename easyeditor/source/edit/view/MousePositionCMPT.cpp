#include "MousePositionCMPT.h"
#include "MousePositionOP.h"
#include "StringHelper.h"

namespace ee
{

MousePositionCMPT::MousePositionCMPT(wxWindow* parent, const std::string& name, 
									 wxWindow* stage_wnd, EditPanelImpl* stage, 
									 bool create_op)
	: EditCMPT(parent, name, stage)
{
	if (create_op) {
		m_editop = new MousePositionOP(stage_wnd, stage, this);
	}
}

void MousePositionCMPT::updatePosition(const sm::vec2& pos)
{
	m_xText->ChangeValue(StringHelper::ToString(pos.x));
	m_yText->ChangeValue(StringHelper::ToString(pos.y));
}

wxSizer* MousePositionCMPT::InitLayout()
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

}