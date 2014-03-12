#ifndef _EASYIMAGE_RECT_CUT_CMPT_H_
#define _EASYIMAGE_RECT_CUT_CMPT_H_

#include <drag2d.h>

namespace eimage
{

class StagePanel;

class RectCutCMPT : public d2d::AbstractEditCMPT
{
public:
	RectCutCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

	void onSaveEditOP(wxCommandEvent& event);
	void onLoadEditOP(wxCommandEvent& event);

protected:
	virtual wxSizer* initLayout();

private:
	wxSizer* initEditIOLayout();
	wxSizer* initDataOutputLayout();

	void onSetImagesPath(wxCommandEvent& event);
	void onSetJsonPath(wxCommandEvent& event);
	void onOutputData(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	wxTextCtrl *m_imagePath, *m_jsonPath;

}; // RectCutCMPT

}

#endif // _EASYIMAGE_RECT_CUT_CMPT_H_