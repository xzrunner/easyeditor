
#ifndef EBUILDER_FRAME_H
#define EBUILDER_FRAME_H

#include <wx/wx.h>

class wxSplitterWindow;

namespace ebuilder
{
	class Frame : public wxFrame
	{
	public:
		Frame(const wxString& title);

	private:
		void onNew(wxCommandEvent& event);
		void onOpen(wxCommandEvent& event);
		void onSave(wxCommandEvent& event);
		void onSaveAs(wxCommandEvent& event);
		void onSaveCodes(wxCommandEvent& event);

		void onQuit(wxCommandEvent& event);
		void onPreview(wxCommandEvent& event);
		void onRefresh(wxCommandEvent& event);
		void onFullScreenStage(wxCommandEvent& event);
		void onSettingGameMenu(wxCommandEvent& event);
		void onSettingOperatingMenu(wxCommandEvent& event);
		void onSettingOutputMenu(wxCommandEvent& event);
		void onAbout(wxCommandEvent& event);

		void initMenuBar();

		wxMenu* initFileBar();
		wxMenu* initViewBar();
		wxMenu* initSettingsBar();
		wxMenu* initHelpBar();

		void initWorkingFrame();

		void clear();

		void setCurrFilename();

	private:
		enum
		{
			myID_MENU_PREVIEW = 1000,
			myID_MENU_SETTINGS_GAME,
			myID_MENU_SETTINGS_OPERATING,
			myID_MENU_SETTINGS_OUTPUT,
			myID_MENU_REFRESH,
			myID_MENU_FULL,
			myID_MENU_SAVE_CODES
		};

		static const float SASH_GRAVITY_HOR;
		static const float SASH_GRAVITY_VER;

	private:
		wxSplitterWindow *m_vSplitter, *m_hLeftSplitter, 
			*m_hRightSplitter;

		wxString m_currFilename;

		DECLARE_EVENT_TABLE()

	}; // Frame
}

#endif // EBUILDER_FRAME_H
