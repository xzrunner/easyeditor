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

	virtual void Load(const char* filepath) override;
	virtual void Store(const char* filepath) const override;

	virtual bool IsDirty() const override;

	virtual void GetAllSprite(std::vector<ee::SprPtr>& sprs) const override {}

	virtual const ee::EditPanel* GetEditPanel() const override;

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	StagePanel* m_stage;

}; // Task

}

#endif // _EASYP3DINV_TASK_H_