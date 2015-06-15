#ifndef _LR_TOOL_BAR_H_
#define _LR_TOOL_BAR_H_

#include <drag2d.h>

namespace lr
{

class ToolBar
{
public:
	ToolBar(wxFrame* frame, int id);

	void SetLibrary(d2d::LibraryPanel* library) { m_library = library; }

	void OnClick(int idx);

private:
	void PopulateToolbar(int id);

private:
	wxToolBarBase* m_toolbar;

	d2d::LibraryPanel* m_library;

}; // ToolBar

}

#endif // _LR_TOOL_BAR_H_