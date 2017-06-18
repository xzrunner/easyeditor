#ifndef _EASYDB_WARN_PANEL_H_
#define _EASYDB_WARN_PANEL_H_

#include <wx/wx.h>

namespace edb
{

class WarnList;

class WarnPanel : public wxPanel
{
public:
	WarnPanel(wxWindow* parent);

	WarnList* GetList() { return m_list; }

private:
	void InitLayout();

private:
	WarnList* m_list;

}; // WarnPanel

}

#endif // _EASYDB_WARN_PANEL_H_