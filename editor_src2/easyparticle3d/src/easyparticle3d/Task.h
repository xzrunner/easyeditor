#ifndef _EASYPARTICLE3D_TASK_H_
#define _EASYPARTICLE3D_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace eparticle3d
{

class LibraryPanel;
class StagePanel;
class ToolbarPanel;

class Task
{
public:

	virtual void loadFromFile(const char* filename);
	virtual void storeToFile(const char* filename) const;

	virtual void clear();

	d2d::GLCanvas* getCanvas() const;

	ToolbarPanel* getToolbar() const {
		return m_toolbar;
	}

	static Task* create(wxFrame* parent)
	{
		return new Task(parent);
	}

protected:
	Task(wxFrame* parent);
	~Task();

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
