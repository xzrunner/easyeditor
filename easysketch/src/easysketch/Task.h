#ifndef _EASYSKETCH_TASK_H_
#define _EASYSKETCH_TASK_H_

#include <ee/Task.h>

class wxFrame;
class wxWindow;

namespace ee { class LibraryPanel; class PropertySettingPanel; class ToolbarPanel; }

namespace esketch
{

class Task : public ee::Task
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void GetAllSprite(std::vector<ee::Sprite*>& sprs) const {}

	virtual const ee::EditPanel* GetEditPanel() const;

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	ee::LibraryPanel* m_library;
	ee::PropertySettingPanel* m_property;
	ee::EditPanel* m_stage;
	ee::ToolbarPanel* m_toolbar;

}; // Task

}

#endif // _EASYSKETCH_TASK_H_