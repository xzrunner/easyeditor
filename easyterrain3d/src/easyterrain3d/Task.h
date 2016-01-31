#ifndef _EASYTERRAIN3D_TASK_H_
#define _EASYTERRAIN3D_TASK_H_


#include <wx/splitter.h>

namespace eterrain3d
{

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

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	ee::LibraryPanel* m_library;
	ee::PropertySettingPanel* m_property;
	ee::EditPanel* m_stage;
	ee::ToolbarPanel* m_toolbar;

}; // Task

}

#endif // _EASYTERRAIN3D_TASK_H_