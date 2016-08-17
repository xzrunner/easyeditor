#include "LibraryPage.h"
#include "config.h"

#include <ee/LibraryList.h>

namespace ep3dinv
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "P3dInv")
{
	InitLayout();
	m_list->SetFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return true;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
}

}