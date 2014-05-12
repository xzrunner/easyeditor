
#ifndef EMODELING_FRAME_H
#define EMODELING_FRAME_H

#include <wx/wx.h>

namespace emodeling
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
		void onAbout(wxCommandEvent& event);

		void onSettingOperatingMenu(wxCommandEvent& event);
		void onSettingViewMenu(wxCommandEvent& event);

		void onPreview(wxCommandEvent& event);

		void onCodeLove2d(wxCommandEvent& event);

		void initMenuBar();

		wxMenu* initFileBar();

		wxMenu* initViewBar();
		wxMenu* initCodesBar();
		wxMenu* initSettingsBar();
		wxMenu* initHelpBar();

		void setCurrFilename();

	private:
		Task* m_task;

		wxString m_currFilename;

		DECLARE_EVENT_TABLE()

	}; // Frame
}

#endif // EMODELING_FRAME_H