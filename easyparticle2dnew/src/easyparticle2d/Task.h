#ifndef _EASYPARTICLE2D_TASK_H_
#define _EASYPARTICLE2D_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace eparticle2d
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

	static Task* create(wxFrame* parent)
	{
		return new Task(parent);
	}

protected:
	Task(wxFrame* parent);
	~Task();

private:
	void initLayout();

	void StorePSSymbol(const char* filename, Json::Value& val) const;
	void LoadPSSymbol(const char* filename, const Json::Value& val);

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	LibraryPanel* m_library;
	StagePanel* m_stage;
	ToolbarPanel* m_toolbar;

	// todo
	std::string m_ps_name;

}; // Task

}

#endif // _EASYPARTICLE2D_TASK_H_
