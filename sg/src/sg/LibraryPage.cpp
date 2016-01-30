#include "LibraryPage.h"

#include <easycomplex.h>

namespace sg
{

LibraryPage::LibraryPage(wxWindow* parent, const char* name)
	: d2d::ILibraryPage(parent, name, true)
{
	InitLayout();
}

bool LibraryPage::IsHandleSymbol(d2d::Symbol* symbol) const
{
	return dynamic_cast<d2d::ImageSymbol*>(symbol) != NULL
		|| dynamic_cast<ecomplex::Symbol*>(symbol) != NULL;
}

}