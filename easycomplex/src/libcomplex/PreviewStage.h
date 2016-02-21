#ifndef _EASY_EASYCOMPLEX_PREVIEW_STAGE_H_
#define _EASY_EASYCOMPLEX_PREVIEW_STAGE_H_

#include <ee/EditPanel.h>
#include <ee/PlayControl.h>

namespace ecomplex
{

class PreviewStage : public ee::EditPanel
{
public:
	PreviewStage(wxWindow* parent, wxTopLevelWindow* frame,
		ee::PlayControl& control);
	
	virtual bool Update(int version);
	
private:
	ee::PlayControl& m_control;

}; // PreviewStage

}

#endif // _EASY_EASYCOMPLEX_PREVIEW_STAGE_H_