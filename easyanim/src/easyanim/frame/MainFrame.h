#pragma once

#include <wx/wx.h>

#include "view/Utility.h"

class wxSplitterWindow;

namespace eanim
{
	class MainFrame : public wxFrame
	{
	public:
		MainFrame(const wxString& title);

	private:
		void onNew(wxCommandEvent& event);
		void onOpen(wxCommandEvent& event);
		void onSave(wxCommandEvent& event);
		void onSaveAs(wxCommandEvent& event);

		void onQuit(wxCommandEvent& event);
		void onAbout(wxCommandEvent& event);
// 		void onSetLanguageCN(wxCommandEvent& event);
// 		void onSetLanguageEN(wxCommandEvent& event);
		void onPreview(wxCommandEvent& event);

		void initMenuBar();
		wxMenu* initFileBar();
		wxMenu* initViewBar();
		wxMenu* initHelpBar();

		void initWorkingFrame();

		void clear();
		void refresh();

		void setCurrFilename();

	private:
		static const float SASH_GRAVITY_HOR;
		static const float SASH_GRAVITY_VERT;

		enum
		{
			Menu_LanguageSetting,
			Menu_LanguageChinese,
			Menu_LanguageEnglish,
			Menu_Preview
		};

		static LanguageEntry entries[];

	private:
		std::string m_currFilename;

		DECLARE_EVENT_TABLE()

	}; // MainFrame
}

