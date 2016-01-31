#ifndef _ESHAPE_TASK_H_
#define _ESHAPE_TASK_H_


#include <wx/splitter.h>

namespace eshape
{

class LibraryPanel;
class StagePanel;
class ToolbarPanel;

class Task : public ee::ITask
{
public:
	Task(wxFrame* parent);
	~Task();

	virtual void load(const char* filename);
	virtual void store(const char* filename) const;

	virtual bool isDirty() const;

	virtual void clear();

	virtual void getAllSprite(std::vector<const ee::ISprite*>& sprites) const {}

	virtual const ee::EditPanel* getEditPanel() const;

private:
	void initWindows(wxSplitterWindow* leftHorizontalSplitter, 
		wxSplitterWindow* leftVerticalSplitter, wxSplitterWindow* rightVerticalSplitter,
		wxWindow*& library, wxWindow*& property, wxWindow*& stage, wxWindow*& toolbar);

	void initLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	LibraryPanel* m_library;
	ee::PropertySettingPanel* m_property;
	StagePanel* m_stage;
	ToolbarPanel* m_toolbar;

}; // Task

}

#endif // _ESHAPE_TASK_H_