#ifndef _EASYIMAGE_AUTO_TRI_CUT_CMPT_H_
#define _EASYIMAGE_AUTO_TRI_CUT_CMPT_H_

#include <drag2d.h>

namespace eimage
{

class StagePanel;

class ExtractOutlineRaw;
class ExtractOutlineFine;

class AutoTriCutCMPT : public d2d::AbstractEditCMPT
{
public:
	AutoTriCutCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

protected:
	virtual wxSizer* initLayout();

private:
	void Trigger(wxCommandEvent& event);
	void OutputOutline(wxCommandEvent& event);

	void CreateOutline(wxCommandEvent& event);
	void ReduceOutlineCount(wxCommandEvent& event);

private:
	void Trigger();

private:
	StagePanel* m_stage;

	ExtractOutlineRaw* m_raw;
	ExtractOutlineFine* m_fine;

}; // AutoTriCutCMPT

}

#endif // _EASYIMAGE_AUTO_TRI_CUT_CMPT_H_