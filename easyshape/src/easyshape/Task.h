#ifndef _EASYSHAPE_TASK_H_
#define _EASYSHAPE_TASK_H_

#include <ee/Task.h>

class wxFrame;

namespace ee { class LibraryPanel; }

namespace eshape
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

	virtual void GetAllSprite(std::vector<ee::SprPtr>& sprs) const override;

	virtual const ee::EditPanel* GetEditPanel() const override;

private:
	void InitLayout(wxFrame* parent);

private:
	ee::LibraryPanel* m_library;

	eshape::StagePanel* m_stage;

}; // Task

}

#endif // _EASYSHAPE_TASK_H_