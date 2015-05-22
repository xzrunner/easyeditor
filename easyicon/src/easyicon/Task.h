#ifndef _EASYICON_TASK_H_
#define _EASYICON_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace eicon
{

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

	virtual const d2d::EditPanel* getEditPanel() const;

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	d2d::LibraryPanel* m_library;
 	d2d::EditPanel* m_stage;

}; // Task

}

#endif // _EASYICON_TASK_H_