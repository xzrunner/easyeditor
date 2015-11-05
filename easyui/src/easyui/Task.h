#ifndef _EASYUI_TASK_H_
#define _EASYUI_TASK_H_

#include <drag2d.h>
#include <wx/splitter.h>

#include "TopPannels.h"

namespace eui
{

class Task : public d2d::ITask
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const;

	virtual void GetAllSprite(std::vector<const d2d::ISprite*>& sprites) const;

	virtual const d2d::EditPanel* GetEditPanel() const;

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;
	wxFrame* m_parent;

	TopPannels m_top_pannels;

}; // Task

}

#endif // _EASYUI_TASK_H_
