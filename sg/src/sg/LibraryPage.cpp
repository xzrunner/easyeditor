#include "LibraryPage.h"

#include <ee/ImageSymbol.h>

#include <easycomplex.h>

namespace sg
{

LibraryPage::LibraryPage(wxWindow* parent, const char* name)
	: ee::LibraryPage(parent, name, true)
{
	InitLayout();
}

bool LibraryPage::IsHandleSymbol(ee::Symbol* symbol) const
{
	return dynamic_cast<ee::ImageSymbol*>(symbol) != NULL
		|| dynamic_cast<ecomplex::Symbol*>(symbol) != NULL;
}

}