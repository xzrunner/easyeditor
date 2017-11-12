#ifndef _EASYPARTICLE2D_TASK_H_
#define _EASYPARTICLE2D_TASK_H_

#include <ee/Task.h>

class wxFrame;
class wxWindow;

namespace eparticle2d
{

class LibraryPanel;
class StagePanel;
class ToolbarPanel;

class Task : public ee::Task
{
public:
	Task(wxFrame* parent);
	~Task();

	virtual void Load(const char* filename) override;
	virtual void Store(const char* filename) const override;

	virtual bool IsDirty() const override;

	virtual void GetAllSprite(std::vector<ee::SprPtr>& sprs) const override;

	virtual const ee::EditPanel* GetEditPanel() const override;

// 	ee::StageCanvas* getCanvas() const;

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	LibraryPanel* m_library;
	StagePanel* m_stage;
	ToolbarPanel* m_toolbar;

	// todo
	std::string m_ps_name;

}; // Task

}

#endif // _EASYPARTICLE2D_TASK_H_
