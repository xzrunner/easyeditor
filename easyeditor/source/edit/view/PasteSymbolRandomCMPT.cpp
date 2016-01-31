#include "PasteSymbolRandomCMPT.h"
#include "PasteSymbolRandomOP.h"
#include "PasteSymbolRandomWidget.h"

namespace ee
{

PasteSymbolRandomCMPT::PasteSymbolRandomCMPT(wxWindow* parent, const std::string& name, 
											 wxWindow* stage_wnd, EditPanelImpl* stage, 
											 MultiSpritesImpl* spritesImpl, 
											 LibraryPanel* libraryPanel)
	: EditCMPT(parent, name, stage)
{
	m_random_widget = new PasteSymbolRandomWidget(this, libraryPanel);
	m_editop = new PasteSymbolRandomOP(stage_wnd, stage, libraryPanel, m_random_widget);
}

wxSizer* PasteSymbolRandomCMPT::InitLayout()
{
	wxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(m_random_widget);
	return topSizer;
}

}