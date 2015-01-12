#pragma once

#include <wx/wx.h>
#include <drag2d.h>

#include "view/Utility.h"

class wxSplitterWindow;

namespace eanim
{
	class Frame : public wxFrame
	{
	public:
		Frame(const wxString& title);

		void initWithFile(const wxString& path);

		d2d::GLCanvas* getCanvas();

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

		void onSettings(wxCommandEvent& event);
		void onSetBackground(wxCommandEvent& event);

		void initMenuBar();
		void InitStatueBar();

		wxMenu* initFileBar();
		wxMenu* initViewBar();
		wxMenu* initSettingsBar();
		wxMenu* initHelpBar();
		wxMenu* initCodesBar();

		void initWorkingFrame();
		wxWindow* createLeftFrame(wxWindow* parent);
		wxWindow* createCenterFrame(wxWindow* parent);
		wxWindow* createRightFrame(wxWindow* parent);

		void clear();
//		void refresh();

		void setCurrFilename();

	private:
		enum
		{
			ID_PREVIEW = 1000,
			ID_SETTINGS,
			ID_BACKGROUND,
			ID_CODESETTING,
			ID_LOVE2D
		};

	private:
		std::string m_currFilename;

		DECLARE_EVENT_TABLE()

	}; // Frame
}

