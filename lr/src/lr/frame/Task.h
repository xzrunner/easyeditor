#ifndef _LR_TASK_H_
#define _LR_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace lr
{

class LibraryPanel;
class StagePanel;
class ToolBar;

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

	void OnFullView();

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;

	wxSplitterWindow *m_left_split, *m_right_split;
	bool m_full_view;
	
	wxFrame* m_parent;

 	LibraryPanel* m_library;
	d2d::PropertySettingPanel* m_property;
	d2d::LayerPanel* m_layer;
 	StagePanel* m_stage;
	d2d::ViewlistPanel* m_viewlist;
	d2d::GroupTreePanel* m_grouptree;

}; // Task

}

#endif // _LR_TASK_H_
