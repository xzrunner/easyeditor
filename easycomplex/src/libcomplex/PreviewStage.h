#ifndef _EASYCOMPLEX_PREVIEW_STAGE_H_
#define _EASYCOMPLEX_PREVIEW_STAGE_H_

#include <drag2d.h>

namespace ecomplex
{

class PreviewStage : public d2d::EditPanel
{
public:
	PreviewStage(wxWindow* parent, wxTopLevelWindow* frame,
		d2d::PlayControl& control);
	
	virtual bool Update(int version);
	
private:
	d2d::PlayControl& m_control;

}; // PreviewStage

}

#endif // _EASYCOMPLEX_PREVIEW_STAGE_H_