#pragma once


#include <wx/splitter.h>

namespace coceditor
{
	class Task
	{
	public:
		
		virtual void loadFromTextFile(const char* filename);
		virtual void storeToTextFile(const char* filename) const;

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

