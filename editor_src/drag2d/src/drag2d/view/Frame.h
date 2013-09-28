#pragma once

#include "interfaces.h"

#include <wx/wx.h>

namespace d2d
{
	class Frame : public wxFrame
	{
	public:
		Frame(const wxString& title, const wxString& filetag);

		void setTask(ITask* task);

	private:
		void onNew(wxCommandEvent& event);
		void onOpen(wxCommandEvent& event);
		void onSave(wxCommandEvent& event);
		void onSaveAs(wxCommandEvent& event);

		void onQuit(wxCommandEvent& event);

		void onClose(wxCloseEvent& event);

		void initMenuBar();

		wxMenu* initFileBar();
		wxMenu* initHelpBar();

		wxString getFileFilter() const;

		void setCurrFilename();

	private:
		ITask* m_task;

		wxString m_filetag;

		wxString m_currFilename;

		DECLARE_EVENT_TABLE()

	}; // Frame 
}