#ifndef _EASYDB_SEARCH_LIST_H_
#define _EASYDB_SEARCH_LIST_H_

#include <wx/treectrl.h>

#include <vector>

namespace edb
{

class Database;
class MainList;

class SearchList : public wxTreeCtrl
{
public:
	SearchList(wxWindow* parent, MainList* main_list, const Database& db);

	void OnSearch(const std::string& str);
	void OnSearchSameImages(float val);

	void Clear() {}

private:
	void OnItemActivated(wxTreeEvent& event);

	void GetSameImages(const std::vector<std::string>& src, float val,
		std::vector<std::vector<int> >& dst);

private:
	enum
	{
		ID_CTRL = 1000,
	};

private:
	wxTreeItemId m_root;

	MainList* m_main_list;

	const Database& m_db;

	DECLARE_EVENT_TABLE()

}; // SearchList

}

#endif // _EASYDB_SEARCH_LIST_H_