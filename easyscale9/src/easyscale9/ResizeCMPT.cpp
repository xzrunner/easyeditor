#include "ResizeCMPT.h"
#include "ResizeOP.h"
#include "StagePanel.h"

namespace escale9
{

ResizeCMPT::ResizeCMPT(wxWindow* parent, const wxString& name, 
					   StagePanel* stage, ToolbarPanel* toolbar)
	: EditCMPT(parent, name, stage->GetStageImpl())
	, m_stage(stage)
{
	m_editOP = new ResizeOP(stage, toolbar, this);
}

void ResizeCMPT::setSize(int width, int height)
{
	m_wSpin->SetValue(width);
	m_hSpin->SetValue(height);
}

float ResizeCMPT::getWidth() const
{
	return m_wSpin->GetValue();
}

float ResizeCMPT::getHeight() const
{
	return m_hSpin->GetValue();
}

wxSizer* ResizeCMPT::InitLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("size"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	wxBoxSizer* wSizer = new wxBoxSizer(wxHORIZONTAL);
	wSizer->Add(new wxStaticText(this, wxID_ANY, wxT("width: ")));
	m_wSpin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, 2, 10000, 300);
	Connect(m_wSpin->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ResizeCMPT::onChangeSize));
	wSizer->Add(m_wSpin);
	sizer->Add(wSizer);

	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
	hSizer->Add(new wxStaticText(this, wxID_ANY, wxT("height: ")));
	m_hSpin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, 2, 10000, 200);
	Connect(m_hSpin->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ResizeCMPT::onChangeSize));
	hSizer->Add(m_hSpin);
	sizer->Add(hSizer);

	return sizer;
}

void ResizeCMPT::onChangeSize(wxSpinEvent& event)
{
	m_stage->rebuildPatchSymbol();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

} // escale9