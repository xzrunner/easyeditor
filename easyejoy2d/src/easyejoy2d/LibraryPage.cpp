#include "LibraryPage.h"
#include "config.h"

namespace eejoy2d
{

LibraryPage::LibraryPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, wxT("Ejoy2D"))
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