#include "LibraryPage.h"
#include "config.h"

namespace epseudo3d
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, wxT("Template"))
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