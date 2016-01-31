#include "PasteSymbolScaleCMPT.h"
#include "PasteSymbolOP.h"
#include "MultiSpritesImpl.h"
#include "LibraryPanel.h"

#include <wx/spinctrl.h>

namespace ee
{

PasteSymbolScaleCMPT::PasteSymbolScaleCMPT(wxWindow* parent, const std::string& name, 
										   wxWindow* stage_wnd, EditPanelImpl* stage, 
										   LibraryPanel* libraryPanel)
	: EditCMPT(parent, name, stage)
	, m_scaleVal(1.0f)
{
	m_editop = new PasteSymbolOP(stage_wnd, stage, libraryPanel, &m_scaleVal);
}

wxSizer* PasteSymbolScaleCMPT::InitLayout()
{
	wxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Ëõ·Å: ")));

		wxSpinCtrl* scaleCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 10, 100, 100);
		Connect(scaleCtrl->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(PasteSymbolScaleCMPT::onChangeScale));
		sizer->Add(scaleCtrl);

		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("%")));
		topSizer->Add(sizer);
	}
	return topSizer;
}

void PasteSymbolScaleCMPT::onChangeScale(wxSpinEvent& event)
{
	m_scaleVal = event.GetValue() * 0.01f;
}

}