#ifndef _EASYMODELING_SETTING_VIEW_DLG_H_
#define _EASYMODELING_SETTING_VIEW_DLG_H_

#include <wx/dialog.h>

#include <memory>

namespace ee { class StageCanvas; }

namespace emodeling
{

class SettingViewDlg : public wxDialog
{
public:
	SettingViewDlg(wxWindow* parent, const std::shared_ptr<ee::StageCanvas>& canvas);

private:
	wxSizer* initDisplayTypePanel();
	wxSizer* initStylePanel();

	void onChangeDisplayType(wxCommandEvent& event);
	void onChangeStyle(wxSpinEvent& event);
	void onChangeDisplayTriangles(wxCommandEvent& event);
	void onChangeDisplayPolygonBound(wxCommandEvent& event);

private:
	std::shared_ptr<ee::StageCanvas> m_canvas;

	wxSpinCtrl* m_ctlPointSize;

}; // SettingViewDlg

}

#endif // _EASYMODELING_SETTING_VIEW_DLG_H_