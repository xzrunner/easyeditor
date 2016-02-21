#ifndef _EASYSHADOW_TASK_H_
#define _EASYSHADOW_TASK_H_

#include <ee/Task.h>

#include <wx/splitter.h>

namespace ee { class LibraryPanel; }

namespace eshadow
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
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	ee::LibraryPanel* m_library;
 	StagePanel* m_stage;

}; // Task

}

#endif // _EASYSHADOW_TASK_H_