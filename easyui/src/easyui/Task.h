#ifndef _EASYUI_TASK_H_
#define _EASYUI_TASK_H_

#include "TopPannels.h"

#include <ee/Task.h>

#include <wx/splitter.h>

namespace eui
{

class Frame;
class UIStagePage;

class Task : public ee::Task
{
public:
	Task(Frame* frame);
	virtual ~Task();

	virtual void Load(const char* filepath);
	virtual void Store(const char* filepath) const;

	virtual bool IsDirty() const { return false; }

	virtual void GetAllSprite(std::vector<const ee::Sprite*>& sprites) const {}

	virtual const ee::EditPanel* GetEditPanel() const;

	UIStagePage* GetSelectedStagePage();

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;
	Frame* m_frame;

	TopPannels m_top_pannels;

}; // Task

}

#endif // _EASYUI_TASK_H_
