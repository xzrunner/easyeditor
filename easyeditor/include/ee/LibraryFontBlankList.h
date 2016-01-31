#ifndef _EASYEDITOR_LIBRARY_FONTBLANK_LIST_H_
#define _EASYEDITOR_LIBRARY_FONTBLANK_LIST_H_

#include "LibraryList.h"

namespace ee
{

class LibraryFontBlankList : public LibraryList
{
public:
	LibraryFontBlankList(LibraryPage* page);

	virtual void OnListDoubleClicked(wxCommandEvent& event);

}; // LibraryFontBlankList

}

#endif // _EASYEDITOR_LIBRARY_FONTBLANK_LIST_H_ 