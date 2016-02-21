#ifndef _EASYMODELING_LIBRARY_LIST_H_
#define _EASYMODELING_LIBRARY_LIST_H_

#include <ee/LibraryList.h>

namespace emodeling
{

class LibraryList : public ee::LibraryList
{
public:
	LibraryList(ee::LibraryPage* page);

	virtual void OnListDoubleClicked(wxCommandEvent& event);

}; // LibraryPage

}

#endif // _EASYMODELING_LIBRARY_LIST_H_