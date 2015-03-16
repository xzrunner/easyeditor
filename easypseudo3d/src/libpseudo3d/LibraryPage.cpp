#include "LibraryPage.h"
#include "config.h"

namespace epseudo3d
{

LibraryPage::LibraryPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, wxT("Template"))
{
	initLayout();
	m_list->setFileter(FILE_TAG);
}

bool LibraryPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return true;
}

void LibraryPage::onAddPress(wxCommandEvent& event)
{
}

}