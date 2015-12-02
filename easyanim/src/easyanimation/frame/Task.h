#ifndef _EASYANIM_TASK_H_
#define _EASYANIM_TASK_H_

#include <drag2d.h>

#include "Widgets.h"
#include "Controller.h"

namespace eanim
{

class StagePanel;
class KeysPanel;
class KeyFrame;

class Task : public d2d::ITask, public d2d::Observer
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void GetAllSprite(std::vector<const d2d::ISprite*>& sprites) const;

	virtual const d2d::EditPanel* GetEditPanel() const;

	Controller* GetController() { return &m_controller; }

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	Widgets m_widgets;

	Controller m_controller;

}; // Task

}

#endif // _EASYANIM_TASK_H_