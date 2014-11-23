#ifndef _EASYSHAPE_TASK_H_
#define _EASYSHAPE_TASK_H_

#include <drag2d.h>
#include <easyshape.h>

namespace eshape
{

class Task : public d2d::ITask
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void load(const char* filepath);
	virtual void store(const char* filepath) const;

	virtual bool isDirty() const;

	virtual void clear();

	virtual void getAllSprite(std::vector<const d2d::ISprite*>& sprites) const;

	virtual const d2d::EditPanel* getEditPanel() const;

private:
	void InitLayout(wxFrame* parent);

private:
	d2d::LibraryPanel* m_library;

	libshape::StagePanel* m_stage;

}; // Task

}

#endif // _EASYSHAPE_TASK_H_