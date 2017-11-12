#ifndef _EASYSKELETON_TASK_H_
#define _EASYSKELETON_TASK_H_

#include <ee/Task.h>

#include <wx/splitter.h>

namespace ee { class LibraryPanel; class PropertySettingPanel; class ViewlistPanel; class GroupTreePanel; }

namespace eskeleton
{

class StagePanel;

class Task : public ee::Task
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath) override;
	virtual void Store(const char* filepath) const override;

	virtual bool IsDirty() const override;

	virtual void GetAllSprite(std::vector<ee::SprPtr>& sprs) const override {}

	virtual const ee::EditPanel* GetEditPanel() const override;

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	ee::LibraryPanel* m_library;
	ee::PropertySettingPanel* m_property;
	eskeleton::StagePanel* m_stage;
	ee::ViewlistPanel* m_viewlist;
	ee::GroupTreePanel* m_grouptree;

}; // Task

}

#endif // _EASYSKELETON_TASK_H_