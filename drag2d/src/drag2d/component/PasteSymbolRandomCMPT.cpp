#include "PasteSymbolRandomCMPT.h"

#include "operator/PasteSymbolRandomOP.h"
#include "widgets/PasteSymbolRandomWidget.h"

namespace d2d
{

PasteSymbolRandomCMPT::PasteSymbolRandomCMPT(wxWindow* parent, const wxString& name, 
											 wxWindow* stage_wnd, EditPanelImpl* stage, 
											 MultiSpritesImpl* spritesImpl, 
											 LibraryPanel* libraryPanel)
	: AbstractEditCMPT(parent, name, stage)
{
	m_randomWidget = new PasteSymbolRandomWidget(this, libraryPanel);
	m_editOP = new PasteSymbolRandomOP(stage_wnd, stage, libraryPanel, m_randomWidget);
}

wxSizer* PasteSymbolRandomCMPT::initLayout()
{
	wxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(m_randomWidget);
	return topSizer;
}

} // d2d