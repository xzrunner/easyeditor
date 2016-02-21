#include "PasteSymbolRandomCMPT.h"
#include "PasteSymbolRandomOP.h"
#include "PasteSymbolRandomWidget.h"

namespace ee
{

PasteSymbolRandomCMPT::PasteSymbolRandomCMPT(wxWindow* parent, const std::string& name, 
											 wxWindow* stage_wnd, EditPanelImpl* stage, 
											 MultiSpritesImpl* sprites_impl, 
											 LibraryPanel* library)
	: EditCMPT(parent, name, stage)
{
	m_random_widget = new PasteSymbolRandomWidget(this, library);
	m_editop = new PasteSymbolRandomOP(stage_wnd, stage, library, m_random_widget);
}

wxSizer* PasteSymbolRandomCMPT::InitLayout()
{
	wxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(m_random_widget);
	return topSizer;
}

}