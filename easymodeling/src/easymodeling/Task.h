#ifndef _EASYMODELING_TASK_H_
#define _EASYMODELING_TASK_H_

#include <wx/splitter.h>

namespace ee { class StageCanvas; }

namespace emodeling
{

class Task
{
public:

	virtual void LoadFromFile(const char* filename);
	virtual void StoreToFile(const char* filename) const;

	virtual void Clear();

	virtual void OnPreview() const;

	ee::StageCanvas* GetCanvas() const;

	static Task* Create(wxFrame* parent)
	{
		return new Task(parent);
	}

protected:
	Task(wxFrame* parent);
	~Task();

	virtual void InitWindows(wxSplitterWindow* leftHorizontalSplitter, 
		wxSplitterWindow* leftVerticalSplitter, wxSplitterWindow* rightVerticalSplitter,
		wxWindow*& library, wxWindow*& property, wxWindow*& stage, wxWindow*& toolbar);

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

}; // Task

}

#endif // _EASYMODELING_TASK_H_
