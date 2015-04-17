#ifndef _EASYUI_LIBRARY_PAGE_H_
#define _EASYUI_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace eui
{

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent, const char* name);

	virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

}; // LibraryPage 

}

#endif // _EASYUI_LIBRARY_PAGE_H_