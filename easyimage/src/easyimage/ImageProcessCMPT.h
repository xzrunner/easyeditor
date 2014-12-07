#ifndef _EASYIMAGE_IMAGE_PROCESS_CMPT_H_
#define _EASYIMAGE_IMAGE_PROCESS_CMPT_H_

#include <drag2d.h>

namespace eimage
{

class StagePanel;

class ImageProcessCMPT : public d2d::AbstractEditCMPT
{
public:
	ImageProcessCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

protected:
	virtual wxSizer* initLayout();

}; // ImageProcessCMPT

}

#endif // _EASYIMAGE_IMAGE_PROCESS_CMPT_H_