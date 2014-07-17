#ifndef _EASYIMAGE_PIXEL_DIFF_CMPT_H_
#define _EASYIMAGE_PIXEL_DIFF_CMPT_H_

#include <drag2d.h>

namespace eimage
{

class StagePanel;

class PixelDiffCMPT : public d2d::AbstractEditCMPT
{
public:
	PixelDiffCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

protected:
	virtual wxSizer* initLayout();

private:
	StagePanel* m_stage;

}; // PixelDiffCMPT

}

#endif // _EASYIMAGE_PIXEL_DIFF_CMPT_H_