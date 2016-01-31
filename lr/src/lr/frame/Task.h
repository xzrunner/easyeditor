#ifndef _LR_TASK_H_
#define _LR_TASK_H_


#include <wx/splitter.h>

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

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void GetAllSprite(std::vector<const ee::Sprite*>& sprites) const;

	virtual const ee::EditPanel* GetEditPanel() const;

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
