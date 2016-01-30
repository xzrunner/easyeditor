#ifndef _ESHADER_TASK_H_
#define _ESHADER_TASK_H_

#include <drag2d.h>

namespace eshader 
{

class LibraryPanel;
class ToolbarPanel;

class Task : public d2d::ITask
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void GetAllSprite(std::vector<const d2d::Sprite*>& sprites) const;

	virtual const d2d::EditPanel* GetEditPanel() const;

private:
	void initLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	bool m_is_2d;

	LibraryPanel* m_library;
	d2d::EditPanel* m_stage;
	ToolbarPanel* m_toolbar;

}; // Task

}

#endif // _ESHADER_TASK_H_