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

	virtual void Load(const char* filepath) override;
	virtual void Store(const char* filepath) const override;

	virtual bool IsDirty() const override;

	virtual void GetAllSprite(std::vector<ee::SprPtr>& sprs) const override;

	virtual const ee::EditPanel* GetEditPanel() const override;

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