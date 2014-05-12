#pragma once

#include <wx/wx.h>

namespace coceditor
{
	class CodeSettingDlg : public wxDialog
	{
	public:
		CodeSettingDlg(wxWindow* parent);

	private:
		void initLayout();
		void initPackerSet(wxSizer* sizer, int index);

		void onSetScale(wxSpinEvent& event);

		void onSelectSymbolFile(wxCommandEvent& event);
		void onSelectSymbolDir(wxCommandEvent& event);

 		void onSelectPackerFile1(wxCommandEvent& event);
 		void onSelectPackerFile2(wxCommandEvent& event);
		void onSelectPackerFile3(wxCommandEvent& event);
		void onSelectPackerFile4(wxCommandEvent& event);

	private:
		static const int NUM = 4;

	private:
		wxTextCtrl* m_symbolPath;

		wxTextCtrl* m_packerPath[NUM];

	}; // CodeSettingDlg
}

