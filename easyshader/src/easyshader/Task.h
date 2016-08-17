#ifndef _ESHADER_TASK_H_
#define _ESHADER_TASK_H_

#include <ee/Task.h>

#include <wx/wx.h>

namespace eshader 
{

class LibraryPanel;
class ToolbarPanel;

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
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	bool m_is_2d;

	LibraryPanel* m_library;
	ee::EditPanel* m_stage;
	ToolbarPanel* m_toolbar;

}; // Task

}

#endif // _ESHADER_TASK_H_