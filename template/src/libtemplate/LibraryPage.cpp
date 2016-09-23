#include "LibraryPage.h"

#include <ee/LibraryList.h>

namespace etemplate
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Template")
{
	InitLayout();
	m_list->SetFileter(ee::FileType::GetTag(ee::FILE_FMT));
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return true;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
}

}