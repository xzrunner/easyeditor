#ifndef _EASYPARTICLE2D_TASK_H_
#define _EASYPARTICLE2D_TASK_H_

#include <drag2d.h>

namespace eparticle2d
{

class LibraryPanel;
class StagePanel;
class ToolbarPanel;

class Task : public d2d::ITask
{
public:
	Task(wxFrame* parent);
	~Task();

	virtual void load(const char* filename);
	virtual void store(const char* filename) const;

	virtual bool isDirty() const;

	virtual void clear();

	virtual void getAllSprite(std::vector<const d2d::ISprite*>& sprites) const;

	virtual const d2d::EditPanel* getEditPanel() const;

// 	d2d::GLCanvas* getCanvas() const;

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
