#ifndef _EASYP3DINV_TASK_H_
#define _EASYP3DINV_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace ep3dinv
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

	virtual const d2d::EditPanel* getEditPanel() const;

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	StagePanel* m_stage;

}; // Task

}

#endif // _EASYP3DINV_TASK_H_