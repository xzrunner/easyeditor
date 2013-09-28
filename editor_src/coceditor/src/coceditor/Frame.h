#pragma once

#include <wx/wx.h>

namespace coceditor
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

		void onPreview(wxCommandEvent& event);

		void onExtractImg(wxCommandEvent& event);

		void onCodeSetting(wxCommandEvent& event);
		void onCodeCOC(wxCommandEvent& event);

		void initMenuBar();

		wxMenu* initFileBar();
		wxMenu* initViewBar();
		wxMenu* initEditBar();
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

