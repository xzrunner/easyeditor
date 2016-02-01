#ifndef _EASYIMAGE_AUTO_TRI_CUT_CMPT_H_
#define _EASYIMAGE_AUTO_TRI_CUT_CMPT_H_

#include <ee/EditCMPT.h>

namespace eimage
{

class StagePanel;

class ExtractOutlineRaw;
class ExtractOutlineFine;

class AutoTriCutCMPT : public ee::EditCMPT
{
public:
	AutoTriCutCMPT(wxWindow* parent, const std::string& name,
		StagePanel* stage);

protected:
	virtual wxSizer* InitLayout();

private:
	void Trigger(wxCommandEvent& event);
	void OutputOutline(wxCommandEvent& event);

	void CreateOutline(wxCommandEvent& event);
	void ReduceOutlineCount(wxCommandEvent& event);

	void OnDebug(wxCommandEvent& event);

private:
	void Trigger();

private:
	StagePanel* m_stage;

	ExtractOutlineRaw* m_raw;
	ExtractOutlineFine* m_fine;

}; // AutoTriCutCMPT

}

#endif // _EASYIMAGE_AUTO_TRI_CUT_CMPT_H_