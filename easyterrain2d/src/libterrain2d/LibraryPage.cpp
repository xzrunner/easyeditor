#include "LibraryPage.h"

namespace eterrain2d
{

LibraryPage::LibraryPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, wxT("Terrain"))
{
	initLayout();
	//	m_list->setFileter(emesh::FILE_TAG);
}

bool LibraryPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return true;
}

void LibraryPage::onAddPress(wxCommandEvent& event)
{
}

}