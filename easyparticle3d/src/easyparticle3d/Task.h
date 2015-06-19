#ifndef _EASYPARTICLE3D_TASK_H_
#define _EASYPARTICLE3D_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace eparticle3d
{

class LibraryPanel;
class StagePanel;
class ToolbarPanel;

class Task : public d2d::ITask
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void load(const char* filepath);
	virtual void store(const char* filepath) const;

	virtual bool isDirty() const;

	virtual void clear();

	virtual void getAllSprite(std::vector<const d2d::ISprite*>& sprites) const {}

	virtual const d2d::EditPanel* getEditPanel() const;

	ToolbarPanel* getToolbar() const {
		return m_toolbar;
	}

	void StoreAsAnim(const wxString& filepath) const;
	void StoreAsInvert(const wxString& filepath) const;

private:
	void initLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	LibraryPanel* m_library;
	StagePanel* m_stage;
	ToolbarPanel* m_toolbar;

}; // Task

}

#endif // _EASYPARTICLE3D_TASK_H_
