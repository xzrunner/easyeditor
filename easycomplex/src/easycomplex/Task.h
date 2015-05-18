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

	virtual const d2d::EditPanel* getEditPanel() const;

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	ecomplex::LibraryPanel* m_library;
	d2d::PropertySettingPanel* m_property;
	ecomplex::StagePanel* m_stage;
	d2d::ViewlistPanel* m_viewlist;
	d2d::GroupTreePanel* m_grouptree;

	friend class FileIO;

}; // Task

}

#endif // _EASYCOMPLEX_TASK_H_