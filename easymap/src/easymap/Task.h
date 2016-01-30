#ifndef _EASYMAP_TASK_H_
#define _EASYMAP_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace emap
{

class StagePanel;

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

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	d2d::LibraryPanel* m_library;
	d2d::PropertySettingPanel* m_property;
 	StagePanel* m_stage;
	d2d::ViewlistPanel* m_viewlist;

}; // Task

}

#endif // _EASYMAP_TASK_H_
