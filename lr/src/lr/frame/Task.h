#ifndef _LR_TASK_H_
#define _LR_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace lr
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

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	LibraryPanel* m_library;
 	StagePanel* m_stage;
	d2d::ViewlistPanel* m_viewlist;

}; // Task

}

#endif // _LR_TASK_H_
