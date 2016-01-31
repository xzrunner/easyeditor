
#ifndef EMODELING_SETTING_VIEW_DLG_H
#define EMODELING_SETTING_VIEW_DLG_H



namespace emodeling
{
	class SettingViewDlg : public wxDialog
	{
	public:
		SettingViewDlg(wxWindow* parent, ee::StageCanvas* canvas);

	private:
		wxSizer* initDisplayTypePanel();
		wxSizer* initStylePanel();

		void onChangeDisplayType(wxCommandEvent& event);
		void onChangeStyle(wxSpinEvent& event);
		void onChangeDisplayTriangles(wxCommandEvent& event);
		void onChangeDisplayPolygonBound(wxCommandEvent& event);

	private:
		ee::StageCanvas* m_canvas;

		wxSpinCtrl* m_ctlPointSize;

	}; // SettingViewDlg
}

#endif // EMODELING_SETTING_VIEW_DLG_H