#pragma once

#include <drag2d.h>
#include <wx/splitter.h>

namespace escale9
{
	class StagePanel;
	class ToolbarPanel;

	class Task : public d2d::ITask
	{
	public:
		Task(wxFrame* parent);
		virtual ~Task();

		virtual void load(const char* filepath);
		virtual void store(const char* filepath) const;

		virtual bool isDirty() const;

		virtual void clear();

		virtual void getAllSprite(std::vector<const d2d::ISprite*>& sprites) const {}

	private:
		void initWindows(wxSplitterWindow* leftHorizontalSplitter, 
			wxSplitterWindow* leftVerticalSplitter, wxSplitterWindow* rightVerticalSplitter,
			wxWindow*& library, wxWindow*& property, wxWindow*& stage, wxWindow*& toolbar);

		void initLayout();

	private:
		wxWindow* m_root;

		wxFrame* m_parent;

		d2d::LibraryPanel* m_library;
		StagePanel* m_stage;
		ToolbarPanel* m_toolbar;

	}; // Task
}

