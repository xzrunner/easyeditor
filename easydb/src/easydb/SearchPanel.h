#ifndef _EASYDB_SEARCH_PANEL_H_
#define _EASYDB_SEARCH_PANEL_H_

#include <wx/wx.h>

namespace edb
{

class SearchList;

class SearchPanel : public wxPanel
{
public:
	SearchPanel(wxWindow* parent);

	SearchList* GetList() { return m_list; }

private:
	void InitLayout();

private:
	SearchList* m_list;

}; // SearchPanel

}

#endif // _EASYDB_SEARCH_PANEL_H_