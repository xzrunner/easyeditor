#ifndef _EASYUI_LIBRARY_PANEL_H_
#define _EASYUI_LIBRARY_PANEL_H_

#include <drag2d.h>

namespace eui
{

class LibraryPanel : public wxPanel
{
public:
	LibraryPanel(wxWindow* parent);

	d2d::LibraryPanel* GetUILibrary() { return m_library_ui; }
	d2d::LibraryPanel* GetRawLibrary() { return m_library_raw; }

	void EnableUILibrary(bool enable);

	void Clear();

private:
	void InitLayout();

private:
	d2d::LibraryPanel* m_library_ui;
	d2d::LibraryPanel* m_library_raw;

}; // LibraryPanel

}

#endif // _EASYUI_LIBRARY_PANEL_H_