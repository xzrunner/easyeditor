#ifndef _EASYIMAGE_AUTO_CUT_CMPT_H_
#define _EASYIMAGE_AUTO_CUT_CMPT_H_

#include <drag2d.h>

namespace eimage
{

class StagePanel;

class AutoCutCMPT : public d2d::AbstractEditCMPT
{
public:
	AutoCutCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

protected:
	virtual wxSizer* initLayout();

private:
	void CreateSymbolEdge(wxCommandEvent& event);

private:
	StagePanel* m_stage;

}; // AutoCutCMPT

}

#endif // _EASYIMAGE_AUTO_CUT_CMPT_H_