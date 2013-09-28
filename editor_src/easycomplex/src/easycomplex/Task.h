#pragma once

#include <drag2d.h>
#include <wx/splitter.h>

namespace ecomplex 
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

	private:
		void initWindows(wxSplitterWindow* leftHorizontalSplitter, 
			wxSplitterWindow* leftVerticalSplitter, wxSplitterWindow* rightVerticalSplitter,
			wxWindow*& library, wxWindow*& property, wxWindow*& stage, wxWindow*& toolbar);

		void initLayout();

	private:
		wxWindow* m_root;

		wxFrame* m_parent;

	}; // Task
}

