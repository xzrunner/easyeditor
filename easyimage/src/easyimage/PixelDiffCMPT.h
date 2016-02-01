#ifndef _EASYIMAGE_PIXEL_DIFF_CMPT_H_
#define _EASYIMAGE_PIXEL_DIFF_CMPT_H_

#include <ee/EditCMPT.h>

namespace eimage
{

class StagePanel;

class PixelDiffCMPT : public ee::EditCMPT
{
public:
	PixelDiffCMPT(wxWindow* parent, const std::string& name,
		StagePanel* stage);

protected:
	virtual wxSizer* InitLayout();

private:
	StagePanel* m_stage;

}; // PixelDiffCMPT

}

#endif // _EASYIMAGE_PIXEL_DIFF_CMPT_H_