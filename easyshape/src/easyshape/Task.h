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

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void GetAllSprite(std::vector<const ee::Sprite*>& sprs) const;

	virtual const ee::EditPanel* GetEditPanel() const;

private:
	void InitLayout(wxFrame* parent);

private:
	ee::LibraryPanel* m_library;

	eshape::StagePanel* m_stage;

}; // Task

}

#endif // _EASYSHAPE_TASK_H_