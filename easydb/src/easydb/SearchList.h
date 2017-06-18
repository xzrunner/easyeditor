#ifndef _EASYDB_SEARCH_LIST_H_
#define _EASYDB_SEARCH_LIST_H_

#include <wx/treectrl.h>

namespace edb
{

class SearchList : public wxTreeCtrl
{
public:
	SearchList(wxWindow* parent);

	void Clear() {}

private:

//	DECLARE_EVENT_TABLE()

}; // SearchList

}

#endif // _EASYDB_SEARCH_LIST_H_