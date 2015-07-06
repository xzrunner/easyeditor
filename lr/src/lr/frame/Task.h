#ifndef _LR_TASK_H_
#define _LR_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace lr
{

class LibraryPanel;
class StagePanel;

class Task : public d2d::ITask
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void Clear();

	virtual void GetAllSprite(std::vector<const d2d::ISprite*>& sprites) const;

	virtual const d2d::EditPanel* GetEditPanel() const;

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	LibraryPanel* m_library;
	d2d::PropertySettingPanel* m_property;
 	StagePanel* m_stage;
	d2d::ViewlistPanel* m_viewlist;
	d2d::GroupTreePanel* m_grouptree;

	d2d::ViewPanelMgr m_view_panel_mgr;

}; // Task

}

#endif // _LR_TASK_H_
