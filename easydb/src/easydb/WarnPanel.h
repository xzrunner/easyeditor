#ifndef _EASYDB_WARN_PANEL_H_
#define _EASYDB_WARN_PANEL_H_

#include <wx/wx.h>

namespace edb
{

class WarnList;
class MainList;
class Database;

class WarnPanel : public wxPanel
{
public:
	WarnPanel(wxWindow* parent, MainList* main_list, const Database& db);

	WarnList* GetList() { return m_list; }

private:
	void InitLayout(MainList* main_list, const Database& db);

private:
	WarnList* m_list;

}; // WarnPanel

}

#endif // _EASYDB_WARN_PANEL_H_