#include "LibraryPage.h"

#include <ee/LibraryList.h>
#include <ee/FileType.h>

namespace ebone
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Bone")
{
	InitLayout();
	m_list->SetFileter(ee::FileType::GetTag(ee::FILE_BONE));
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return true;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
}

}