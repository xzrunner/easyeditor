#include "LibraryPage.h"

#include <ee/LibraryList.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

namespace libanim2
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Anim2")
{
	InitLayout();
	m_list->SetFileter(ee::SymbolFile::Instance()->Tag(s2::SYM_ANIM2));
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return true;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
}

}