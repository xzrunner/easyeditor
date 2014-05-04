#ifndef FORMATION_TASK_H
#define FORMATION_TASK_H

#include <drag2d.h>
#include <wx/splitter.h>

namespace formation
{
	class Task
	{
	public:

		virtual void loadFromFile(const char* filename);
		virtual void storeToFile(const char* filename) const;

		virtual void clear();

		static Task* create(wxFrame* parent)
		{
			return new Task(parent);
		}

	protected:
		Task(wxFrame* parent);
		~Task();

		virtual void initWindows(wxSplitterWindow* leftHorizontalSplitter, 
			wxSplitterWindow* leftVerticalSplitter, wxSplitterWindow* rightVerticalSplitter,
			wxWindow*& library, wxWindow*& property, wxWindow*& stage, wxWindow*& toolbar);

	private:
		void initLayout();

	private:
		wxWindow* m_root;

		wxFrame* m_parent;

	}; // Task
}

#endif // FORMATION_TASK_H