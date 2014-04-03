#ifndef _ESHAPE_TASK_H_
#define _ESHAPE_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace eshape
{

class StagePanel;

class Task :public d2d::ITask
{
public:
	Task(wxFrame* parent);
	~Task();

	virtual void load(const char* filename);
	virtual void store(const char* filename) const;

	virtual bool isDirty() const;

	virtual void clear();

	virtual void getAllSprite(std::vector<const d2d::ISprite*>& sprites) const {}

	virtual const d2d::EditPanel* getEditPanel() const;

private:
	void initWindows(wxSplitterWindow* leftHorizontalSplitter, 
		wxSplitterWindow* leftVerticalSplitter, wxSplitterWindow* rightVerticalSplitter,
		wxWindow*& library, wxWindow*& property, wxWindow*& stage, wxWindow*& toolbar);

	void initLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	StagePanel* m_stage;

}; // Task

}

#endif // _ESHAPE_TASK_H_