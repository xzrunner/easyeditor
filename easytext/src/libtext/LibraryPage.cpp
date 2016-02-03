#include "LibraryPage.h"
#include "config.h"
#include "Symbol.h"

#include <ee/LibraryList.h>
#include <ee/FileType.h>

namespace etext
{

LibraryPage::LibraryPage(wxWindow* parent)
	: ee::LibraryPage(parent, "Text")
{
	InitLayout();
	m_list->SetFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	std::string type = ee::FileType::GetTag(ee::FileType::e_text);
	ee::LibraryPage::OnAddPress(type);
}

}