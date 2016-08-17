#ifndef _EASYMASK_TASK_H_
#define _EASYMASK_TASK_H_

#include <ee/Task.h>

#include <wx/splitter.h>

namespace ee { class PropertySettingPanel; }

namespace emask
{

class LibraryPanel;
class StagePanel; 
class ToolbarPanel;

class Task : public ee::Task
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void GetAllSprite(std::vector<const ee::Sprite*>& sprs) const {}

	virtual const ee::EditPanel* GetEditPanel() const;

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	LibraryPanel*	m_library;
 	StagePanel*		m_stage;
	ToolbarPanel*	m_toolbar;

}; // Task

}

#endif // _EASYMASK_TASK_H_