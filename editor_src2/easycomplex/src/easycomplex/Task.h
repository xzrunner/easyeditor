#ifndef _EASYCOMPLEX_TASK_H_
#define _EASYCOMPLEX_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace ecomplex 
{

class LibraryPanel; 
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

	virtual void getAllSprite(std::vector<const d2d::ISprite*>& sprites) const;

	ecomplex::StagePanel* getStagePanel() { return m_stage; }

private:
	void initWindows(wxSplitterWindow* leftHorizontalSplitter, 
		wxSplitterWindow* leftVerticalSplitter, wxSplitterWindow* rightVerticalSplitter,
		wxWindow*& library, wxWindow*& property, wxWindow*& stage, wxWindow*& toolbar);

	void initLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	ecomplex::LibraryPanel* m_library;
	d2d::PropertySettingPanel* m_property;
	ecomplex::StagePanel* m_stage;
	d2d::ViewlistPanel* m_viewlist;

	friend class FileIO;

}; // Task

}

#endif // _EASYCOMPLEX_TASK_H_