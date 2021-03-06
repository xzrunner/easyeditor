#ifndef SHOOTBUBBLE_TASK_H_
#define SHOOTBUBBLE_TASK_H_


#include <wx/splitter.h>

namespace shootbubble
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

#endif // SHOOTBUBBLE_TASK_H_