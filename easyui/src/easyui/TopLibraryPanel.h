#ifndef _EASYUI_TOP_LIBRARY_PANEL_H_
#define _EASYUI_TOP_LIBRARY_PANEL_H_

#include <drag2d.h>

namespace eui
{

class TopLibraryPanel : public wxPanel
{
public:
	TopLibraryPanel(wxWindow* parent);

	d2d::LibraryPanel* GetUILibrary() { return m_library_ui; }
	d2d::LibraryPanel* GetRawLibrary() { return m_library_raw; }

	void EnableUILibrary(bool enable);

	void Clear();

private:
	void InitLayout();

private:
	d2d::LibraryPanel* m_library_ui;
	d2d::LibraryPanel* m_library_raw;

}; // TopLibraryPanel

}

#endif // _EASYUI_TOP_LIBRARY_PANEL_H_