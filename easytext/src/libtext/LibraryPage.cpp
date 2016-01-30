#include "LibraryPage.h"
#include "config.h"
#include "Symbol.h"

namespace etext
{

LibraryPage::LibraryPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, "Text")
{
	InitLayout();
	m_list->SetFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(d2d::Symbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string type = d2d::FileType::GetTag(d2d::FileType::e_text);
	d2d::ILibraryPage::OnAddPress(type);
}

}