#ifndef _TEST_DTEX_TASK_H_
#define _TEST_DTEX_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

namespace tdtex
{

class Task : public d2d::ITask
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void GetAllSprite(std::vector<const d2d::Sprite*>& sprites) const {}

	virtual const d2d::EditPanel* GetEditPanel() const;

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	d2d::LibraryPanel* m_library;
 	d2d::EditPanel* m_stage;

}; // Task

}

#endif // _TEST_DTEX_TASK_H_