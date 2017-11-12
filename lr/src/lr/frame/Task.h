#ifndef _LR_TASK_H_
#define _LR_TASK_H_

#include <ee/Task.h>

#include <wx/wx.h>
#include <wx/splitter.h>

namespace ee { class PropertySettingPanel; class LayerPanel; class ViewlistPanel; class GroupTreePanel; }

namespace lr
{

class LibraryPanel;
class StagePanel;
class ToolBar;

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
	ee::PropertySettingPanel* m_property;
	ee::LayerPanel* m_layer;
 	StagePanel* m_stage;
	ee::ViewlistPanel* m_viewlist;
	ee::GroupTreePanel* m_grouptree;

}; // Task

}

#endif // _LR_TASK_H_
