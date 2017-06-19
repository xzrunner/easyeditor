#ifndef _EASYDB_SEARCH_PANEL_H_
#define _EASYDB_SEARCH_PANEL_H_

#include <wx/wx.h>

namespace edb
{

class SearchList;
class MainList;
class Database;

class SearchPanel : public wxPanel
{
public:
	SearchPanel(wxWindow* parent, MainList* main_list, const Database& db);

	SearchList* GetList() { return m_list; }

private:
	void InitLayout(MainList* main_list, const Database& db);

private:
	SearchList* m_list;

}; // SearchPanel

}

#endif // _EASYDB_SEARCH_PANEL_H_