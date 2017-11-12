#ifndef _EASYANIM_TASK_H_
#define _EASYANIM_TASK_H_

#include <ee/Task.h>
#include <ee/Observer.h>

class wxFrame;
class wxWindow;

namespace eanim
{

class StagePanel;
class KeysPanel;
class KeyFrame;

class Task : public ee::Task, public ee::Observer
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath) override;
	virtual void Store(const char* filepath) const override;

	virtual bool IsDirty() const override;

	virtual void GetAllSprite(std::vector<ee::SprPtr>& sprs) const override;

	virtual const ee::EditPanel* GetEditPanel() const override;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;
	wxFrame* m_parent;

}; // Task

}

#endif // _EASYANIM_TASK_H_