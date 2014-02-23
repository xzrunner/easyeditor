#ifndef _EASYMESH_TASK_H_
#define _EASYMESH_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace emesh
{

class StagePanel;

class Task : public d2d::ITask
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void load(const char* filepath);
	virtual void store(const char* filepath) const;

	virtual bool isDirty() const;

	virtual void clear();

	virtual void getAllSprite(std::vector<const d2d::ISprite*>& sprites) const {}

private:
	void initWindows(wxSplitterWindow* leftHorizontalSplitter, 
		wxSplitterWindow* leftVerticalSplitter, wxSplitterWindow* rightVerticalSplitter,
		wxWindow*& library, wxWindow*& property, wxWindow*& stage, wxWindow*& toolbar);

	void initLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	d2d::LibraryPanel* m_library;
	d2d::PropertySettingPanel* m_property;
	emesh::StagePanel* m_stage;

}; // Task

}

#endif // _EASYMESH_TASK_H_