#ifndef _EASYMAP_TASK_H_
#define _EASYMAP_TASK_H_


#include <wx/splitter.h>

namespace emap
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

	virtual void GetAllSprite(std::vector<const ee::Sprite*>& sprites) const;

	virtual const ee::EditPanel* GetEditPanel() const;

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	ee::LibraryPanel* m_library;
	ee::PropertySettingPanel* m_property;
 	StagePanel* m_stage;
	ee::ViewlistPanel* m_viewlist;

}; // Task

}

#endif // _EASYMAP_TASK_H_
