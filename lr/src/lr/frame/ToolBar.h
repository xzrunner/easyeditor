#ifndef _LR_TOOL_BAR_H_
#define _LR_TOOL_BAR_H_

namespace ee { class LibraryPanel; }

class wxFrame;
class wxToolBarBase;

namespace lr
{

class ToolBar
{
public:
	ToolBar(wxFrame* frame, int id);

	void SetLibrary(ee::LibraryPanel* library) { m_library = library; }

	void OnClick(int idx);

private:
	void PopulateToolbar(int id);

private:
	wxToolBarBase* m_toolbar;

	ee::LibraryPanel* m_library;

}; // ToolBar

}

#endif // _LR_TOOL_BAR_H_