#ifndef _EASYTERRAIN3D_TASK_H_
#define _EASYTERRAIN3D_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace eterrain3d
{

class Task : public d2d::ITask
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void load(const char* filepath);
	virtual void store(const char* filepath) const;

	virtual bool isDirty() const;

	virtual void clear();

	virtual void getAllSprite(std::vector<const d2d::ISprite*>& sprites) const {}

	virtual const d2d::EditPanel* getEditPanel() const;

private:
	void initLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	d2d::LibraryPanel* m_library;
	d2d::PropertySettingPanel* m_property;
	d2d::EditPanel* m_stage;
	d2d::ToolbarPanel* m_toolbar;

}; // Task

}

#endif // _EASYTERRAIN3D_TASK_H_