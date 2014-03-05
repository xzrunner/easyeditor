#include "ResizeCMPT.h"
#include "ResizeOP.h"
#include "Context.h"
#include "StagePanel.h"

namespace escale9
{

ResizeCMPT::ResizeCMPT(wxWindow* parent, const wxString& name, 
					   d2d::EditPanel* editPanel)
	: AbstractEditCMPT(parent, name, editPanel)
{
	m_editOP = new ResizeOP(editPanel);
}

void ResizeCMPT::setValue(int width, int height)
{
	m_wSpin->SetValue(width);
	m_hSpin->SetValue(height);
}

wxSizer* ResizeCMPT::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("size"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	wxBoxSizer* wSizer = new wxBoxSizer(wxHORIZONTAL);
	wSizer->Add(new wxStaticText(this, wxID_ANY, wxT("width: ")));
	m_wSpin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, 10, 10000, 300);
	Connect(m_wSpin->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ResizeCMPT::onChangeSize));
	wSizer->Add(m_wSpin);
	sizer->Add(wSizer);

	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
	hSizer->Add(new wxStaticText(this, wxID_ANY, wxT("height: ")));
	m_hSpin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, 10, 10000, 200);
	Connect(m_hSpin->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ResizeCMPT::onChangeSize));
	hSizer->Add(m_hSpin);
	sizer->Add(hSizer);

	return sizer;
}

void ResizeCMPT::onChangeSize(wxSpinEvent& event)
{
	Context* context = Context::Instance();
	context->width = m_wSpin->GetValue();
	context->height = m_hSpin->GetValue();
	context->stage->rebuildPatchSymbol();
	context->stage->Refresh();
}
} // escale9