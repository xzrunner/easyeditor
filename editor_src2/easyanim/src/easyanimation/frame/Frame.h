#pragma once

#include <wx/wx.h>

#include "view/Utility.h"

class wxSplitterWindow;

namespace eanim
{
	class Frame : public wxFrame
	{
	public:
		Frame(const wxString& title);

		void initWithFile(const wxString& path);

	private:
		void onNew(wxCommandEvent& event);
		void onOpen(wxCommandEvent& event);
		void onSave(wxCommandEvent& event);
		void onSaveAs(wxCommandEvent& event);

		void onQuit(wxCommandEvent& event);
		void onAbout(wxCommandEvent& event);
		void onPreview(wxCommandEvent& event);

		void onCodeSetting(wxCommandEvent& event);
		void onCodeLove2d(wxCommandEvent& event);

		void initMenuBar();
		wxMenu* initFileBar();
		wxMenu* initViewBar();
		wxMenu* initHelpBar();
		wxMenu* initCodesBar();

		void initWorkingFrame();

		void clear();
//		void refresh();

		void setCurrFilename();

	private:
		enum
		{
			ID_PREVIEW = 1000,
			ID_CODESETTING,
			ID_LOVE2D
		};

	private:
		std::string m_currFilename;

		DECLARE_EVENT_TABLE()

	}; // Frame
}

