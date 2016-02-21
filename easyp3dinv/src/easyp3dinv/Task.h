#ifndef _EASYP3DINV_TASK_H_
#define _EASYP3DINV_TASK_H_

#include <ee/Task.h>

#include <wx/splitter.h>

namespace ep3dinv
{

class StagePanel;

class Task : public ee::Task
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void GetAllSprite(std::vector<const ee::Sprite*>& sprites) const {}

	virtual const ee::EditPanel* GetEditPanel() const;

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	StagePanel* m_stage;

}; // Task

}

#endif // _EASYP3DINV_TASK_H_