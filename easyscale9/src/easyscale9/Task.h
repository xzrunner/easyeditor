#ifndef _EASYSCALE9_TASK_H_
#define _EASYSCALE9_TASK_H_

#include <wx/splitter.h>

#include <ee/Task.h>

namespace ee { class LibraryPanel; class PropertySettingPanel; }

namespace escale9
{

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
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	ee::LibraryPanel* m_library;
	ee::PropertySettingPanel* m_property;
	StagePanel* m_stage;
	ToolbarPanel* m_toolbar;

}; // Task

}

#endif // _EASYSCALE9_TASK_H_