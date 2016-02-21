#ifndef _EASYTERRAIN2D_TASK_H_
#define _EASYTERRAIN2D_TASK_H_

#include <ee/Task.h>

#include <wx/wx.h>

namespace ee { class LibraryPanel; }

namespace eterrain2d
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

	virtual void GetAllSprite(std::vector<const ee::Sprite*>& sprites) const {}

	virtual const ee::EditPanel* GetEditPanel() const;

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	ee::LibraryPanel* m_library;
 	StagePanel* m_stage;
	ToolbarPanel* m_toolbar;

}; // Task

}

#endif // _EASYTERRAIN2D_TASK_H_