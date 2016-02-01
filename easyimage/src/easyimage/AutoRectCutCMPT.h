#ifndef _EASYIMAGE_AUTO_RECT_CUT_CMPT_H_
#define _EASYIMAGE_AUTO_RECT_CUT_CMPT_H_

#include <ee/EditCMPT.h>

namespace eimage
{

class StagePanel;

class AutoRectCutCMPT : public ee::EditCMPT
{
public:
	AutoRectCutCMPT(wxWindow* parent, const std::string& name,
		StagePanel* stage);

protected:
	virtual wxSizer* InitLayout();

	//void OnAddRect(wxCommandEvent& event);
	void OnCreateRects(wxCommandEvent& event);

	void OnOutputRects(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	//wxChoice *m_width_choice, *m_height_choice;

}; // AutoRectCutCMPT

}

#endif // _EASYIMAGE_AUTO_RECT_CUT_CMPT_H_