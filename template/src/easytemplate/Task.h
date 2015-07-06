#ifndef _EASYTEMPLATE_TASK_H_
#define _EASYTEMPLATE_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace etemplate
{

class Task : public d2d::ITask
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void Clear();

	virtual void GetAllSprite(std::vector<const d2d::ISprite*>& sprites) const {}

	virtual const d2d::EditPanel* GetEditPanel() const;

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	d2d::LibraryPanel* m_library;
	d2d::PropertySettingPanel* m_property;
 	d2d::EditPanel* m_stage;

	d2d::ViewPanelMgr m_view_panel_mgr;

}; // Task

}

#endif // _EASYTEMPLATE_TASK_H_