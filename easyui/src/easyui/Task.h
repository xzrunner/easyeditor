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

	virtual void Load(const char* filepath) override;
	virtual void Store(const char* filepath) const override;

	virtual bool IsDirty() const override { return false; }

	virtual void GetAllSprite(std::vector<ee::SprPtr>& sprs) const override {}

	virtual const ee::EditPanel* GetEditPanel() const override;

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
