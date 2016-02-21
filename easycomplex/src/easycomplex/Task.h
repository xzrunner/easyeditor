#ifndef _EASYCOMPLEX_TASK_H_
#define _EASYCOMPLEX_TASK_H_

#include <ee/Task.h>

#include <wx/splitter.h>

namespace ee { class ViewlistPanel; class GroupTreePanel; }

namespace ecomplex 
{

class LibraryPanel; 
class PropertySettingPanel;
class StagePanel;

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

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

	void LoadGroupTree(const char* filepath);
	void StoreGroupTree(const char* filepath) const;

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	ecomplex::LibraryPanel* m_library;
	PropertySettingPanel* m_property;
	ecomplex::StagePanel* m_stage;
	ee::ViewlistPanel* m_viewlist;
	ee::GroupTreePanel* m_grouptree;

	friend class FileIO;

}; // Task

}

#endif // _EASYCOMPLEX_TASK_H_