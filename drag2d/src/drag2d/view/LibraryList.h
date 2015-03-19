#ifndef _DRAG2D_LIBRARY_LIST_H_
#define _DRAG2D_LIBRARY_LIST_H_

#include "widgets/VerticalImageList.h"

namespace d2d
{

class ISymbol;
class ILibraryPage;

class LibraryList : public VerticalImageList
{
public:
	LibraryList(ILibraryPage* page, const wxString& filter = wxT("symbol"),
		bool draggable = true);

	ListItem* getItem(int index = -1) const;
	ISymbol* getSymbol(int index = -1) const;

	void reloadTexture() const;

private:
	ILibraryPage* m_page;

}; // LibraryList

}

#endif // _DRAG2D_LIBRARY_LIST_H_