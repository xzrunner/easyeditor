#pragma once

#include <drag2d.h>
#include <wx/splitter.h>

namespace libcomplex { class LibraryPanel; class StagePanel; }

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

		libcomplex::StagePanel* getStagePanel() { return m_stage; }

	private:
		void initWindows(wxSplitterWindow* leftHorizontalSplitter, 
			wxSplitterWindow* leftVerticalSplitter, wxSplitterWindow* rightVerticalSplitter,
			wxWindow*& library, wxWindow*& property, wxWindow*& stage, wxWindow*& toolbar);

		void initLayout();

	private:
		wxWindow* m_root;

		wxFrame* m_parent;

		libcomplex::LibraryPanel* m_library;
		d2d::PropertySettingPanel* m_property;
		libcomplex::StagePanel* m_stage;
		d2d::ViewlistPanel* m_viewlist;

		friend class FileIO;

	}; // Task
}

