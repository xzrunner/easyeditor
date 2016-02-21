#ifndef _EASYUI_TOP_LIBRARY_PANEL_H_
#define _EASYUI_TOP_LIBRARY_PANEL_H_

#include <wx/panel.h>

namespace ee { class LibraryPanel; }

namespace eui
{

class TopLibraryPanel : public wxPanel
{
public:
	TopLibraryPanel(wxWindow* parent);

	ee::LibraryPanel* GetUILibrary() { return m_library_ui; }
	ee::LibraryPanel* GetRawLibrary() { return m_library_raw; }

	void EnableUILibrary(bool enable);

	void Clear();

private:
	void InitLayout();

private:
	ee::LibraryPanel* m_library_ui;
	ee::LibraryPanel* m_library_raw;

}; // TopLibraryPanel

}

#endif // _EASYUI_TOP_LIBRARY_PANEL_H_