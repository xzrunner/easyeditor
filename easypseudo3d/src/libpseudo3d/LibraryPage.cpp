#include "LibraryPage.h"
#include "config.h"

#include <ee/LibraryList.h>

namespace epseudo3d
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Template")
{
	InitLayout();
	m_list->SetFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* symbol) const
{
	return true;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
}

}