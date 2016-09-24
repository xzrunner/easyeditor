#include "LibraryPage.h"

#include <ee/LibraryList.h>
#include <ee/FileType.h>

namespace eskeleton
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Skeleton")
{
	InitLayout();
	m_list->SetFileter(ee::FileType::GetTag(ee::FILE_SKELETON));
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return true;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
}

}