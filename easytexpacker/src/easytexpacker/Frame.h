#ifndef _EASYTEXPACKER_FRAME_H_
#define _EASYTEXPACKER_FRAME_H_

#include <wx/wx.h>

namespace etexpacker
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

		void onQuit(wxCommandEvent& event);
		//		void onAbout(wxCommandEvent& event);

		void onCodeLove2d(wxCommandEvent& event);

		void initMenuBar();
		void initStatueBar();

		wxMenu* initFileBar();
		wxMenu* initCodesBar();
		wxMenu* initHelpBar();

		void clear();

		void setCurrFilename();

	private:
		Task* m_task;

		wxString m_currFilename;

		DECLARE_EVENT_TABLE()

	}; // Frame
}

#endif // _EASYTEXPACKER_FRAME_H_