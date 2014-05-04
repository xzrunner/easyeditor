#ifndef _EASYPARTICLE2D_TASK_H_
#define _EASYPARTICLE2D_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace eparticle2d
{

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

private:
	wxWindow* m_root;

	wxFrame* m_parent;

}; // Task

}

#endif // _EASYPARTICLE2D_TASK_H_
