#ifndef _EASYTERRAIN2D_TASK_H_
#define _EASYTERRAIN2D_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace eterrain2d
{

class StagePanel;
class ToolbarPanel;

class Task : public d2d::ITask
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void GetAllSprite(std::vector<const d2d::ISprite*>& sprites) const {}

	virtual const d2d::EditPanel* GetEditPanel() const;

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	d2d::LibraryPanel* m_library;
 	StagePanel* m_stage;
	ToolbarPanel* m_toolbar;

}; // Task

}

#endif // _EASYTERRAIN2D_TASK_H_