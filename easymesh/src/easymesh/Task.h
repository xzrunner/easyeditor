#ifndef _EASYMESH_TASK_H_
#define _EASYMESH_TASK_H_


#include <wx/splitter.h>

namespace emesh
{

class StagePanel;

class Task : public ee::Task
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void GetAllSprite(std::vector<const ee::Sprite*>& sprites) const {}

	virtual const ee::EditPanel* GetEditPanel() const;

	emesh::StagePanel* getStagePanel() { return m_stage; }

private:
	void initWindows(wxSplitterWindow* leftHorizontalSplitter, 
		wxSplitterWindow* leftVerticalSplitter, wxSplitterWindow* rightVerticalSplitter,
		wxWindow*& library, wxWindow*& property, wxWindow*& stage, wxWindow*& toolbar);

	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	ee::LibraryPanel* m_library;
	ee::PropertySettingPanel* m_property;
	emesh::StagePanel* m_stage;

}; // Task

}

#endif // _EASYMESH_TASK_H_