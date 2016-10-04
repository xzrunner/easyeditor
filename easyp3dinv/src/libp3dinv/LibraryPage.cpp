#include "LibraryPage.h"
#include "config.h"

#include <ee/LibraryList.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

namespace ep3dinv
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "P3dInv")
{
	InitLayout();
	m_list->SetFileter(ee::SymbolFile::Instance()->Tag(ee::SYM_P3DINV));
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* sym) const
{
	return true;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
}

}