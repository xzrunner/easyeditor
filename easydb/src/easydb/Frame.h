#ifndef EDB_FRAME_H
#define EDB_FRAME_H

#include <wx/wx.h>

namespace edb
{
	class Task;

	class Frame : public wxFrame
	{
	public:
		Frame(const wxString& title);

	private:
		void onNew(wxCommandEvent& event);
		void onOpen(wxCommandEvent& event);
		void onSave(wxCommandEvent& event);
		void onSaveAs(wxCommandEvent& event);

		void onConnect(wxCommandEvent& event);

		void onQuit(wxCommandEvent& event);

		void initMenuBar();

		wxMenu* initFileBar();
		wxMenu* initHelpBar();

		void clear();

		void setCurrFilename();

	private:
		enum
		{
			ID_CONNECT = 1000
		};

	private:
		Task* m_task;

		wxString m_currFilename;

		DECLARE_EVENT_TABLE()

	}; // Frame
}

#endif // EDB_FRAME_H