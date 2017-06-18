#ifndef _EASYDB_MAIN_PANEL_H_
#define _EASYDB_MAIN_PANEL_H_

#include <wx/wx.h>

namespace edb
{

class MainList;

class MainPanel : public wxPanel
{
public:
	MainPanel(wxWindow* parent);

	MainList* GetList() { return m_list; }

private:
	void InitLayout();

private:
	MainList* m_list;

}; // MainPanel

}

#endif // _EASYDB_MAIN_PANEL_H_