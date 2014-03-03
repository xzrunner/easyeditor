#ifndef RAIDEN_FRAME_H
#define RAIDEN_FRAME_H

#include <wx/wx.h>

namespace raiden
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

		void onPreview(wxCommandEvent& event);
		void onQuit(wxCommandEvent& event);

		void initMenuBar();

		wxMenu* initFileBar();
		wxMenu* initViewBar();
		wxMenu* initHelpBar();

		void clear();

		void setCurrFilename();

	private:
		enum
		{
			Menu_Preview = 500
		};

	private:
		Task* m_task;

		wxString m_currFilename;

		DECLARE_EVENT_TABLE()

	}; // Frame
}

#endif // RAIDEN_FRAME_H