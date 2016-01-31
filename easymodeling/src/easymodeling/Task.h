
#ifndef EMODELING_TASK_H
#define EMODELING_TASK_H


#include <wx/splitter.h>

namespace emodeling
{
	class Task
	{
	public:

		virtual void loadFromFile(const char* filename);
		virtual void storeToFile(const char* filename) const;

		virtual void clear();

		virtual void onPreview() const;

		ee::StageCanvas* getCanvas() const;

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
		void InitLayout();

	private:
		wxWindow* m_root;

		wxFrame* m_parent;

	}; // Task
}

#endif // EMODELING_TASK_H
