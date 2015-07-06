#ifndef _EASYSKETCH_TASK_H_
#define _EASYSKETCH_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace esketch
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
	void initLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	d2d::LibraryPanel* m_library;
	d2d::PropertySettingPanel* m_property;
	d2d::EditPanel* m_stage;
	d2d::ToolbarPanel* m_toolbar;

}; // Task

}

#endif // _EASYSKETCH_TASK_H_