#ifndef _EASYIMAGE_RECT_CUT_CMPT_H_
#define _EASYIMAGE_RECT_CUT_CMPT_H_



namespace eimage
{

class StagePanel;

class RectCutCMPT : public ee::EditCMPT
{
public:
	RectCutCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

	void onSaveEditOP(wxCommandEvent& event);
	void onLoadEditOP(wxCommandEvent& event);

protected:
	virtual wxSizer* InitLayout();

private:
	wxSizer* initEditIOLayout();
	wxSizer* initDataOutputLayout();
	wxSizer* initAddRectLayout();

	void onSetImagesPath(wxCommandEvent& event);
	void onSetJsonPath(wxCommandEvent& event);
	void onOutputData(wxCommandEvent& event);
	void onAddRect(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	wxTextCtrl *m_imagePath, *m_jsonPath;

	wxTextCtrl *m_widthCtrl, *m_heightCtrl;

}; // RectCutCMPT

}

#endif // _EASYIMAGE_RECT_CUT_CMPT_H_