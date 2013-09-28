#include "PasteSymbolRandomCMPT.h"

#include "operator/PasteSymbolRandomOP.h"
#include "widgets/PasteSymbolRandomWidget.h"

namespace d2d
{

PasteSymbolRandomCMPT::PasteSymbolRandomCMPT(wxWindow* parent, const wxString& name, 
											 EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
											 LibraryPanel* libraryPanel)
	: AbstractEditCMPT(parent, name, editPanel)
{
	m_randomWidget = new PasteSymbolRandomWidget(this, libraryPanel);
	m_editOP = new PasteSymbolRandomOP(editPanel, spritesImpl, libraryPanel, m_randomWidget);
}

wxSizer* PasteSymbolRandomCMPT::initLayout()
{
	wxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(m_randomWidget);
	return topSizer;
}

} // d2d