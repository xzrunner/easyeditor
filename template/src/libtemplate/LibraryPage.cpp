#include "LibraryPage.h"

#include <ee/LibraryList.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

namespace etemplate
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Template")
{
	InitLayout();
	m_list->SetFileter(ee::SymbolFile::Instance()->Tag(s2::SYM_FMT));
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return true;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
}

}