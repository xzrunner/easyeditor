#include "LibraryPage.h"

#include <ee/LibraryList.h>
#include <ee/FileType.h>

namespace etrail
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Trail")
{
	InitLayout();
	m_list->SetFileter(ee::FileType::GetTag(ee::FileType::e_trail));
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* symbol) const
{
	return true;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
}

}