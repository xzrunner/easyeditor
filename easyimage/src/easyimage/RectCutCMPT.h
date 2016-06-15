#ifndef _EASYIMAGE_RECT_CUT_CMPT_H_
#define _EASYIMAGE_RECT_CUT_CMPT_H_

#include <ee/EditCMPT.h>

namespace eimage
{

class StagePanel;

class RectCutCMPT : public ee::EditCMPT
{
public:
	RectCutCMPT(wxWindow* parent, const std::string& name,
		StagePanel* stage);

	void OnSaveEditOP(wxCommandEvent& event);
	void OnLoadEditOP(wxCommandEvent& event);

protected:
	virtual wxSizer* InitLayout();

private:
	wxSizer* InitEditIOLayout();
	wxSizer* InitDataOutputLayout();
	wxSizer* InitAddRectLayout();

	void OnSetImagesPath(wxCommandEvent& event);
	void OnSetJsonPath(wxCommandEvent& event);
	void OnOutputData(wxCommandEvent& event);
	void OnAddRect(wxCommandEvent& event);
	void OnAutoCreateRects(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	wxTextCtrl *m_imagePath, *m_jsonPath;

	wxTextCtrl *m_widthCtrl, *m_heightCtrl;

}; // RectCutCMPT

}

#endif // _EASYIMAGE_RECT_CUT_CMPT_H_