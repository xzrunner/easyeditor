#include "LibraryPage.h"
#include "config.h"

namespace etemplate
{

LibraryPage::LibraryPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, wxT("Template"))
{
	InitLayout();
	m_list->SetFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(d2d::Symbol* symbol) const
{
	return true;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
}

}