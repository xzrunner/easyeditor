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

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void GetAllSprite(std::vector<const d2d::Sprite*>& sprites) const {}

	virtual const d2d::EditPanel* GetEditPanel() const;

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	StagePanel* m_stage;

}; // Task

}

#endif // _EASYP3DINV_TASK_H_